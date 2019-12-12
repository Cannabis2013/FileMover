#include "testfilecreator.h"

TestFileCreator::TestFileCreator(const QString &filePath,
                                 const QStringList &fileNames,
                                 const bool &persist):
    currentWorkingPath(filePath),isPersisted(persist)
{

    if(!persist)
    {
        _initial_fileNames = fileNames;
        return;
    }

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

        file.write("This is a testfile containing only dummy content");

        file.close();

        _initial_fileNames << fp;
    }
}

QStringList TestFileCreator::fileNames() const
{
    return _initial_fileNames;
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

void TestFileCreator::pertainFileList(const QString &directory)
{
    QDir dir(directory);
    if(!dir.exists())
        throw "Directory doesn't exist";

    QDirIterator it(directory,
                    QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot,
                    QDirIterator::Subdirectories);

    QStringList resultingList;

}
