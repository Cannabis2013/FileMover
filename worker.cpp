#include "Worker.h"

bool Worker::moveRecursively(QString path, QString destination)
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

bool Worker::copyRecursively(QString path, QString destination)
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

void Worker::checkAndCorrectForBackslash(QString *path)
{
    if(!path->endsWith("\\"))
        path->append("\\");
}

QString Worker::checkAndCorrectForBackslash(QString path)
{
    if(!path.endsWith("\\"))
        return path + "\\";
    else
        return path;
}

Worker::Worker(QObject *parent):
    QObject(parent)
{

}

double Worker::convertSizeToAppropriateUnits(long long numb, QString &denote, int dec)
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

Worker::~Worker()
{
    delete this;
}

QString Worker::mergeStringList(const QStringList strings)
{
    if(strings.empty())
        return QString();
    else if(strings.count() == 1)
        return strings.first();

    QString result;
    for(QString string : strings)
        result += string + ";";
    return result;
}

QStringList Worker::splitString(const QString split)
{
    QString tempString;
    QStringList splittetList;
    int lastLetter = split.count() -1;
    for(int i = 0;i<split.count();i++)
    {
        QChar w = split.at(i);
        if(w != ';' && lastLetter != i)
            tempString.append(w);
        else
        {
            splittetList << tempString + w;
            tempString.clear();
        }
    }
    return splittetList;
}
