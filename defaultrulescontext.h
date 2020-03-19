#ifndef DEFAULTRULESCONTEXT_H
#define DEFAULTRULESCONTEXT_H

#include "TypeDefinitions.h"
#include "iruleinterfaces.h"

namespace DefaultRulesContext
{
    static QString ruleSizeLimitsToString(const IDefaultRuleCondition *sRule)
    {
        QString minSize = QString::number(sRule->sizeInterval().first.first),
                maxSize = QString::number(sRule->sizeInterval().second.first);
        QString sizeUnitMin = sRule->sizeInterval().first.second,
                sizeUnitMax = sRule->sizeInterval().second.second;
        return "Min: " + minSize + " " + sizeUnitMin
                + " " + "max: " + maxSize + " " + sizeUnitMax;
    }

    static QString ruleDateLimitsToString(const IDefaultRuleCondition *sRule)
    {
        QString startDate = sRule->dateIntervals().first.date().toString("dd.MM.yyyy"),
                endDate = sRule->dateIntervals().second.date().toString("dd.MM.yyyy");
        return "Start dato: " + startDate + " slut dato: " + endDate;
    }

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

    static QString ruleKeyWordToString(const IDefaultRuleCondition *sRule)
    {
        if(sRule->criteria() == DefaultRulesContext::FileSizeMode &&
                sRule->compareCriteria() != DefaultRulesContext::Interval)
            return QString::number(sRule->sizeLimit().first) + " " + sRule->sizeLimit().second;
        else if(sRule->criteria() == DefaultRulesContext::FileSizeMode &&
                sRule->compareCriteria() == DefaultRulesContext::Interval)
            return DefaultRulesContext::ruleSizeLimitsToString(sRule);
        else if((sRule->criteria() == DefaultRulesContext::FileCreatedMode || sRule->criteria() == DefaultRulesContext::FileModifiedMode) &&
                sRule->compareCriteria() != DefaultRulesContext::Interval)
            return sRule->date().toString("dd.MM.yyyy");
        else if((sRule->criteria() == DefaultRulesContext::FileCreatedMode || sRule->criteria() == DefaultRulesContext::FileModifiedMode) &&
                sRule->compareCriteria() == DefaultRulesContext::Interval)
            return DefaultRulesContext::ruleDateLimitsToString(sRule);
        else
            return DefaultRulesContext::mergeStringList(sRule->keyWords());
    }
}

#endif // DEFAULTRULESCONTEXT_H
