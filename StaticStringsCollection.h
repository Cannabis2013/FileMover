#ifndef STATICSTRINGSCOLLECTION_H
#define STATICSTRINGSCOLLECTION_H

#include "rules.h"

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

    // Rule operations
    static QString ruleKeyWordToString(SubRule sRule)
    {
        if(sRule.criteria() == RRT::fileSize &&
                sRule.compareCriteria() != RRT::interval)
            return QString::number(sRule.sizeLimit().first) + " " + sRule.sizeLimit().second;
        else if(sRule.criteria() == RRT::fileSize &&
                sRule.compareCriteria() == RRT::interval)
            return StaticStringCollections::ruleSizeLimitsToString(sRule);
        else if((sRule.criteria() == RRT::fileCreatedMode || sRule.criteria() == RRT::fileModifiedMode) &&
                sRule.compareCriteria() != RRT::interval)
            return sRule.date().toString("dd.MM.yyyy");
        else if((sRule.criteria() == RRT::fileCreatedMode || sRule.criteria() == RRT::fileModifiedMode) &&
                sRule.compareCriteria() == RRT::interval)
            return StaticStringCollections::ruleDateLimitsToString(sRule);
        else
            return StaticStringCollections::mergeStringList(sRule.keyWords());
    }
    static QString ruleSizeLimitsToString(SubRule sRule)
    {
        QString minSize = QString::number(sRule.sizeInterval().first.first),
                maxSize = QString::number(sRule.sizeInterval().second.first);
        QString sizeUnitMin = sRule.sizeInterval().first.second,
                sizeUnitMax = sRule.sizeInterval().second.second;
        return "Min: " + minSize + " " + sizeUnitMin
                + " " + "max: " + maxSize + " " + sizeUnitMax;
    }
    static QString ruleDateLimitsToString(SubRule sRule)
    {
        QString startDate = sRule.dateIntervals().first.date().toString("dd.MM.yyyy"),
                endDate = sRule.dateIntervals().second.date().toString("dd.MM.yyyy");
        return "Start dato: " + startDate + " slut dato: " + endDate;
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
