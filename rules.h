#ifndef RULES_H
#define RULES_H

#include <fileworkeroperator.h>
#include <QPair>
#include <QStringList>
#include "mydatetime.h"

struct SubRule
{
    // Enumerated values..
    rD::copyMode copymode = rD::noMode;
    rD::compareMode fileCompareMode = rD::noCompareModeSet;
    Worker::iteratorMode typeMode = Worker::noTypeSet;
    rD::fileFieldCondition fieldCondition = rD::nonConditionalMode;

    QPair<int,QString>sizeLimit;
    QPair<QPair<int,QString>,QPair<int,QString>> sizeIntervalLimits;
    QPair<rD::compareMode,myDateTime>fixedDate;
    QPair<myDateTime,myDateTime>intervalDate;
    bool matchWholeWords = false;
    QStringList keyWords;

    // Methods..

    QString sizeLimitsToString()
    {
        QString minSize = QString::number(sizeIntervalLimits.first.first),
                maxSize = QString::number(sizeIntervalLimits.second.first);
        QString sizeUnitMin = sizeIntervalLimits.first.second,
                sizeUnitMax = sizeIntervalLimits.second.second;
        return "Min: " + minSize + " " + sizeUnitMin
                + " " + "max: " + maxSize + " " + sizeUnitMax;
    }
    QString dateLimitsToString()
    {
        QString startDate = intervalDate.first.date().toString("dd.MM.yyyy"),
                endDate = intervalDate.second.date().toString("dd.MM.yyyy");
        return "Start dato: " + startDate + " slut dato: " + endDate;
    }
    QString Keyword()
    {
        if(fieldCondition == rD::sizeMode &&
                fileCompareMode != rD::interval)
            return QString::number(sizeLimit.first) + " " + sizeLimit.second;
        else if(fieldCondition == rD::sizeMode &&
                fileCompareMode == rD::interval)
            return sizeLimitsToString();
        else if((fieldCondition == rD::dateCreatedMode || fieldCondition == rD::dateModifiedMode) &&
                fileCompareMode != rD::interval)
            return fixedDate.second.toString("dd.MM.yyyy");
        else if((fieldCondition == rD::dateCreatedMode || fieldCondition == rD::dateModifiedMode) &&
                fileCompareMode == rD::interval)
            return dateLimitsToString();
        else if(fieldCondition == rD::typeMode)
            return rD::typeFromEnum(typeMode);
        else
            return Worker::mergeStringList(keyWords);
    }
};

struct Rule
{
    QString identification,title = "title";
    rD::fileActionRule actionRule;
    QStringList destinationPath;
    QString appliesToPath = "Alle";
    bool deepScanMode = false;
    QList<SubRule> subRules;
    rD::fileActionRule actionFromString();
};


#endif // RULES_H
