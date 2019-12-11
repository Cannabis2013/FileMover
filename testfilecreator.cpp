#include "testfilecreator.h"

TestFileCreator::TestFileCreator(const QString &filePath, const QStringList &fileNames, const bool &persist)
{
    if(!persist && (_fileNames = fileNames) == fileNames)
        return;

    QString fPath = filePath;

    if(filePath.lastIndexOf('/') != filePath.count() - 1)
        fPath += '\\';
    if(fPath == "c:\\" || fPath.contains("c:\\Windows"))
        throw  "Root or system folder not allowed";

    isPersisted = true;

    for(QString fileName : fileNames)
    {
        QString fp = filePath.lastIndexOf('/') == filePath.count() - 1 ?
                    filePath + fileName :
                    filePath + "/" + fileName;

        QFile file(fileName);
        if(!file.open(QIODevice::WriteOnly))
            continue;

        file.write("This is a testfile containing only dummy content");

        file.close();

        _fileNames << fp;
    }
}

QStringList TestFileCreator::fileNames() const
{
    return _fileNames;
}
