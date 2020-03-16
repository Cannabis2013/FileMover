#ifndef STATICSTRINGSCOLLECTION_H
#define STATICSTRINGSCOLLECTION_H

#include <qstringlist.h>

class StaticStringCollections
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
};

#endif // STATICSTRINGSCOLLECTION_H
