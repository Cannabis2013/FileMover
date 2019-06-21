#include "Worker.h"

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

bool Worker::containsTrailingBackslash(QString path)
{
    int lastIndex = path.length() - 1;
    if(path.at(lastIndex) == '/' || path.at(lastIndex) == '\\')
        return true;
    else
        return false;
}

QString Worker::directoryName(QString path)
{
    QString result;

    if(containsTrailingBackslash(path))
        path.remove(path.length() -1,1);

    for (int i = path.length()-1; i >= 0; --i) {
        QChar c = path.at(i);
        if(c == '/' || c == '\\')
            i = -1;
        else {
            result.prepend(c);
        }
    }
    return result;
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

qint64 Worker::byteConvert(int unit, QString fromUnit)
{
    if(fromUnit == "b")
        return unit;
    else if(fromUnit == "kb")
        return unit*1024;
    else if(fromUnit == "mb")
        return unit*1024*1024;
    else if(fromUnit == "gb")
        return unit*1024*1024*1024;
    else
        return unit;
}
