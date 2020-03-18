#include "testfilecreator.h"

TestFileCreator::TestFileCreator()
{
    fillDateMappings();
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

Virtual_Objects *TestFileCreator::createFiles(const QString &directory, const QStringList &fileNames)
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

VIRTUAL_FILE_OBJECT TestFileCreator::VirtualObject(const QString &fileName)
{
    for (int i = 0; i < _virtualObjects.count(); ++i) {
        VIRTUAL_FILE_OBJECT object = _virtualObjects[i];
        QString objectFilePath = object.filePath;
        QString objectFileName = object.fileName();
        if(objectFileName == fileName && QFile::exists(objectFilePath))
            return object;
    }

    throw "Object has no representation on filesystem";
}

Virtual_Objects TestFileCreator::VirtualObjects(const QString &directory)
{
    /*
     * Retrieve a list of virtual file objects conditioned upon their existance in the list
     */
    if(!QFile::exists(directory))
        throw new std::domain_error("Directory is not part of the domain!");

    Virtual_Objects resultingList;
    QDirIterator it(directory,QDir::AllEntries, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QFileInfo file = it.next();
        try {
            resultingList << _virtualObjects.getVirtualObjectFromFilePath(file.absoluteFilePath());
        }  catch (const char *msg) {
            continue;
        }
    }

    return resultingList;
}

void TestFileCreator::fillDateMappings()
{
    for (int i = 0; i < NUMBER_OF_DATE_MAPPINGS; ++i)
    {
        int year = qrand() % (QDate::currentDate().year() % 1000) + MIN_YEAR;
        int month = qrand() % 12 + 1;
        int day = (month == 2) ? qrand() % 29 + 1 :
                               (month % 2 != 0 || month == 8) ? qrand() % 31 + 1 :
                                                                qrand() % 30 + 1;

        dateMappings[i] = QDateTime(QDate(day,month,year));
    }
}

void TestFileCreator::appendVirtualFileObject(const QFileInfo &file)
{
    int index = qrand() % NUMBER_OF_DATE_MAPPINGS + 1;

    VIRTUAL_FILE_OBJECT f_obj;
    f_obj.filePath = file.absoluteFilePath();
    f_obj.dateCreated = dateMappings[index];
    f_obj.additionalInformation = file;

    _virtualObjects << f_obj;
}

Virtual_Objects TestFileCreator::virtualObjects() const
{
    return _virtualObjects;
}


Virtual_Objects::Virtual_Objects()
{

}

VIRTUAL_FILE_OBJECT Virtual_Objects::getVirtualObjectFromFilePath(const QString &path) const
{
    VIRTUAL_FILE_OBJECT item;
    for (int i = 0; i < _objects.count(); ++i) {
        const VIRTUAL_FILE_OBJECT obj = _objects.at(i);
        if(obj.filePath == path)
            return obj;
    }

    throw "Not found";
}

VIRTUAL_FILE_OBJECT Virtual_Objects::getVirtualObjectFromIndex(const int &index) const
{
    if(index < 0 || index >= _objects.count())
        throw std::out_of_range("OUT_OF_RANGE");

    return _objects.value(index);
}

VIRTUAL_FILE_OBJECT Virtual_Objects::getVirtualObjectFromFileName(const QString &name)
{
    VIRTUAL_FILE_OBJECT item;
    for (int i = 0; i < _objects.count(); ++i) {
        const VIRTUAL_FILE_OBJECT obj = _objects.at(i);
        if(obj.fileName() == name)
            return obj;
    }

    throw "Not found";
}

const Virtual_Objects &Virtual_Objects::operator<<(const VIRTUAL_FILE_OBJECT &obj)
{
    _objects << obj;
    return *this;
}

bool Virtual_Objects::operator==(Virtual_Objects objects) const
{
    if(_objects.count() != objects.count())
        return false;

    for (VIRTUAL_FILE_OBJECT obj : _objects) {
        try {
            objects.getVirtualObjectFromFileName(obj.fileName());
        }  catch (char *msg) {
            Q_UNUSED(msg)
            return false;
        }
    }
    return true;
}

bool Virtual_Objects::operator!=(Virtual_Objects objects) const
{
    if(_objects.count() != objects.count())
        return true;

    for (VIRTUAL_FILE_OBJECT obj : _objects) {
        try {
            objects.getVirtualObjectFromFileName(obj.fileName());
        }  catch (char *msg) {
            Q_UNUSED(msg)
            return true;
        }
    }
    return false;
}

VIRTUAL_FILE_OBJECT Virtual_Objects::operator[](long a)
{
    return _objects[a];
}
