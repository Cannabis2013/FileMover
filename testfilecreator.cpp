#include "testfilecreator.h"

TestFileCreator::TestFileCreator(const QString &filePath,
                                 const QStringList &fileNames,
                                 const bool &persist):
    currentWorkingPath(filePath),isPersisted(persist)
{
    QString fPath = filePath;

    if(filePath.lastIndexOf('/') != filePath.count() - 1)
        fPath += '/';
    if(fPath == "c:/" || fPath.contains("c:/Windows"))
        throw  "Root or system folder not allowed";

    QDir dir(filePath);
    if(!dir.exists())
        dir.mkdir(filePath);


    for(QString fileName : fileNames)
    {
        QString fp = filePath.lastIndexOf('/') == filePath.count() - 1 ?
                    filePath + fileName :
                    filePath + "/" + fileName;

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

bool TestFileCreator::emptyTestFolder(QString dirPath)
{
    bool result = true;
    QString directory = dirPath != QString() ? dirPath : currentWorkingPath;

    QDir dir(directory);
    if(!dir.exists())
        throw "Directory doesn't exist";


    QDirIterator it(directory,
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

    f_objects << f_obj;
}
