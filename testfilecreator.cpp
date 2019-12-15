#include "testfilecreator.h"

TestFileCreator::TestFileCreator()
{
}

bool TestFileCreator::emptyTestFolder(const QString &dirPath)
{
    bool result = true;

    QDir dir(dirPath);
    if(!dir.exists())
        throw "Directory doesn't exist";


    QDirIterator it(dirPath,
                    QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot,
                    QDirIterator::Subdirectories);


    while (it.hasNext())
    {
        QFileInfo fInfo = it.next();
        if(fInfo.isFile())
            result = QFile::remove(fInfo.absoluteFilePath()) ? result : false;
        else if(fInfo.isDir())
        {

            QDir dir(fInfo.absoluteFilePath());
            bool operation_succeded = emptyTestFolder(fInfo.absoluteFilePath());
            if(operation_succeded)
                result = dir.rmdir(fInfo.absoluteFilePath()) ? result : false;
            else
                result = dir.removeRecursively() ? result : false;
        }
    }
    return true;

}

void TestFileCreator::createFiles(const QString &directory, const QStringList &fileNames)
{
    QString fPath = directory;

    if(directory.lastIndexOf('/') != directory.count() - 1)
        fPath += '/';
    if(fPath == "c:/" || fPath.contains("c:/Windows"))
        throw  "Root or system folder not allowed";

    QDir dir(directory);
    if(!dir.exists())
        dir.mkdir(directory);


    for(QString fileName : fileNames)
    {
        QString fp = directory.lastIndexOf('/') == directory.count() - 1 ?
                    directory + fileName :
                    directory + "/" + fileName;

        QFile file(fp);
        if(!file.open(QIODevice::WriteOnly))
            continue;

        int count = qrand() % 4096 + 1; // Ensure each file size is within the range 49 - 200.704 bytes
        for (int i = 0; i < count; ++i)
            file.write(dummyContent.toLocal8Bit());

        file.close();

        appendVirtualFileObject(QFileInfo(file));
    }
}

VirtualObjects TestFileCreator::getVirtualFiles(const QString &directory)
{
    /*
     * Retrieve a list of virtual file objects conditioned upon they exists in list
     */


    VirtualObjects resultingList;
    QDirIterator it(directory,QDir::AllEntries, QDirIterator::Subdirectories);
    for (QFileInfo file = it.next(); it.hasNext();file = it.next())
    {
        try {
            resultingList << _virtualObjects.value(file.absoluteFilePath());
        }  catch (char *msg) {
            cout << msg << endl;
        }
    }

    return resultingList;
}

void TestFileCreator::fillDateMappings()
{
    int minYear = 2000;
    for (int i = 0; i < NUMBER_OF_MAPPINGS; ++i)
    {
        int year = qrand() % (QDate::currentDate().year() % 1000) + minYear;
        int month = qrand() % 12 + 1;
        int day = 1;
        if(month == 2)
            day = qrand() % 29 + 1;
        else if(month % 2 != 0 || month == 8)
            day = qrand() % 31 + 1;
        else
            day = qrand() % 30 + 1;

        dateMappings[i] = myDateTime(day,month,year);
    }
}

void TestFileCreator::appendVirtualFileObject(const QFileInfo &file)
{
    int index = qrand() % NUMBER_OF_MAPPINGS + 1;

    VIRTUAL_FILE_OBJECT f_obj;
    f_obj.filePath = file.absoluteFilePath();
    f_obj.dateCreated = dateMappings[index];
    f_obj.additionalInformation = file;

    _virtualObjects << f_obj;
}

VirtualObjects TestFileCreator::virtualObjects() const
{
    return _virtualObjects;
}

VirtualObjects::VirtualObjects()
{

}

const   VIRTUAL_FILE_OBJECT VirtualObjects::value(const QString &path)
{
    VIRTUAL_FILE_OBJECT item;
    for (int i = 0; i < _objects.count(); ++i) {
        const VIRTUAL_FILE_OBJECT obj = _objects.at(i);
        if(obj.filePath == path)
            return obj;
    }

    throw "Not found";
}

void VirtualObjects::operator<<(const VIRTUAL_FILE_OBJECT &obj)
{
    _objects << obj;
}

bool VirtualObjects::operator==(VirtualObjects objects)
{
    for (VIRTUAL_FILE_OBJECT obj : _objects) {
        try {
            objects.value(obj.filePath);
        }  catch (char *msg) {
            Q_UNUSED(msg)
            return false;
        }
    }
    return true;
}

VIRTUAL_FILE_OBJECT VirtualObjects::operator[](int a)
{
    if(a < 0 || a > _objects.count())
        throw std::out_of_range("Index out of bounds");

    return _objects.value(a);
}
