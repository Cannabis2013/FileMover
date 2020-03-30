#ifndef STATICFILEHELPEROPERATIONS_H
#define STATICFILEHELPEROPERATIONS_H

#include <QStringList>
#include <math.h>

namespace FilesContext {
    enum sizeComparemode {LesserThan = 0, LesserOrEqualThan = 1, Equal = 2, greaterOrEqualThan = 3, greaterThan = 4};
    enum dateCompareMode {YoungerThan = 0, YoungerOrExactThan = 1, Exact = 2, OlderOrExtactThan = 3, OlderThan = 4};
    enum fileDateMode {DateCreated = 0, DateEdited = 1};
    enum FileType {Folder = 0x060, File = 0x061,All = 0x062, unresolved = 0x01};
    enum IteratorMode {NonRecursive = 0x080, Recursive = 0x081};

    class FilesContextUtilities;
}

class FilesContextUtilities
{
public:
    static QString mergeStringList(const QStringList strings)
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
    static QStringList splitString(const QString split)
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

    static QString checkAndCorrectForBackslash(const QString &path)
    {
        return !path.endsWith("\\") ? path + "\\" : path;
    }

    static bool hasTrailingBackslash(const QString &string)
    {
        int lastIndex = string.length() - 1;
        if(string.at(lastIndex) == '/' || string.at(lastIndex) == '\\')
            return true;
        else
            return false;
    }

    static double convertFromBytes(quint64 numb, QString &denote, int dec = 0)
    {
        quint16 d = 1,
                dc = 10,
                sizeKb = 1024,
                sizeMb = sizeKb *1024,
                sizeGb = sizeMb * 1024;

        double byteSize = 1024, byteX, result;

        if(dec != 0)
        {
            for(int a = 0;a <dec;a++)
            {
                d *= dc;
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

    static qint64 convertToBytes(const uint &to, const QString &from)
    {
        if(from == "b")
            return to;
        else if(from == "kb")
            return to*1024;
        else if(from == "mb")
            return to*1024*1024;
        else if(from == "gb")
            return to*1024*1024*1024;
        else
            return to;
    }

    static QString directoryName(const QString &path)
    {
        QString p = path;
        QString result;

        if(hasTrailingBackslash(path))
            p.remove(path.length() -1,1);

        for (int i = p.length()-1; i >= 0; --i) {
            QChar c = p.at(i);
            if(c == '/' || c == '\\')
                i = -1;
            else {
                result.prepend(c);
            }
        }
        return result;
    }
};

typedef FilesContextUtilities FCU;

#endif // STATICFILEHELPEROPERATIONS_H
