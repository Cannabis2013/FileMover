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
                result = false;
        }
    }
    return result;

}

const VirtualObjects *TestFileCreator::createFiles(const QString &directory, const QStringList &fileNames)
{
    QString fPath = directory;

#ifdef __WIN64__
    if(directory.lastIndexOf('/') != directory.count() - 1)
        fPath += '/';
    if(fPath == "c:/" || fPath.contains("c:/Windows"))
        throw  "Root or system folder not allowed";
#elif __GNUC__
    if(directory.lastIndexOf('/') != directory.count() - 1)
        fPath += '/';
    if(fPath == '/')
        throw  "Root or system folders not allowed";
#endif
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
            file.write(DUMMY_CONTENT);

        file.close();

        appendVirtualFileObject(QFileInfo(file));
    }

    return  &_virtualObjects;
}

VirtualObjects TestFileCreator::getVirtualFiles(const QString &directory)
{
    /*
     * Retrieve a list of virtual file objects conditioned upon their existance in the list
     */

    VirtualObjects resultingList;
    QDirIterator it(directory,QDir::AllEntries, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QFileInfo file = it.next();
        try {
            resultingList << _virtualObjects.value(file.absoluteFilePath());
        }  catch (const char *msg) {
            cout << msg << endl;
            continue;
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

VIRTUAL_FILE_OBJECT VirtualObjects::value(const QString &path) const
{
    VIRTUAL_FILE_OBJECT item;
    for (int i = 0; i < _objects.count(); ++i) {
        const VIRTUAL_FILE_OBJECT obj = _objects.at(i);
        if(obj.filePath == path)
            return obj;
    }

    throw "Not found";
}

VIRTUAL_FILE_OBJECT VirtualObjects::value(const int &index) const
{
    if(index < 0 || index >= _objects.count())
        throw std::out_of_range("OUT_OF_RANGE");

    return _objects.value(index);
}

void VirtualObjects::operator<<(const VIRTUAL_FILE_OBJECT &obj)
{
    _objects << obj;
}

bool VirtualObjects::operator==(VirtualObjects objects) const
{
    if(_objects.count() != objects.count())
        return false;

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
