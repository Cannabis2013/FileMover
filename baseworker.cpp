#include "baseworker.h"

bool baseWorker::moveRecursively(QString path, QString destination)
{
    bool result = true;
    QDirIterator iT(path,QDir::NoDotAndDotDot | QDir::AllEntries);
    QDir dir(destination);
    if(dir.exists())
        dir.mkdir(destination);
    while(iT.hasNext())
    {
        bool success = false;
        QFileInfo fileInfo = iT.next();
        QString entryName = fileInfo.fileName();
        QString tempDest = checkAndCorrectForBackslash(destination) + entryName;
        if(fileInfo.isDir())
        {
            QDir iDir = fileInfo.absoluteDir();
            if(moveRecursively(fileInfo.absoluteFilePath(),tempDest))
            {
                iDir.removeRecursively();
                success = true;
            }
            result = success ? result : success;
        }
        else if(fileInfo.isFile())
        {
            if(QFile::copy(fileInfo.absoluteFilePath(),tempDest))
            {
                if(QFile::remove(fileInfo.absoluteFilePath()))
                    success = true;
            }
        }
        result = success ? result : success;
    }
    return result;
}

bool baseWorker::copyRecursively(QString path, QString destination)
{
    bool result = true;
    QDirIterator iT(path,QDir::NoDotAndDotDot | QDir::AllEntries);
    QDir dir(destination);
    if(dir.exists())
        dir.mkdir(destination);
    while(iT.hasNext())
    {
        bool success = false;
        QFileInfo fileInfo = iT.next();
        QString entryName = fileInfo.fileName();
        QString tempDest = checkAndCorrectForBackslash(destination) + entryName;
        if(fileInfo.isDir())
        {
            if(copyRecursively(fileInfo.absoluteFilePath(),tempDest))
                success = true;
            result = success ? result : success;
        }
        else if(fileInfo.isFile())
        {
            if(QFile::copy(fileInfo.absoluteFilePath(),tempDest))
                success = true;
        }
        result = success ? result : success;
    }
    return result;
}

void baseWorker::checkAndCorrectForBackslash(QString *path)
{
    if(!path->endsWith("\\"))
        path->append("\\");
}

QString baseWorker::checkAndCorrectForBackslash(QString path)
{
    if(!path.endsWith("\\"))
        return path + "\\";
    else
        return path;
}

baseWorker::baseWorker(QObject *parent):
    QObject(parent)
{

}

double baseWorker::convertSizeToAppropriateUnits(long long numb, QString &denote, int dec)
{
    long long d = 1,
        dc = 10,
        sizeKb = 1024,
        sizeMb = sizeKb *1024,
        sizeGb = sizeMb * 1024;

    double byteSize = 1024, byteX, result;

    if(dec != 0)
    {
        for(int a = 0;a <dec;a++)
        {
            d = d *dc;
        }
    }
    if(numb <sizeKb)
    {
        byteX = numb;
        denote = "bytes";
    }
    else if(numb > sizeKb && numb <sizeMb)
    {
        byteX = numb / byteSize;
        denote = "kb";
    }
    else if(numb > sizeMb && numb < sizeGb)
    {
        byteX = (numb /byteSize) / byteSize;
        denote = "mb";
    }
    else
    {
        byteX = ((numb / byteSize) / byteSize) / byteSize;
        denote = "gb";
    }
    return result = round(byteX *d) / d;
}

baseWorker::~baseWorker()
{
    delete this;
}
