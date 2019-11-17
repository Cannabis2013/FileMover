#ifndef RULES_H
#define RULES_H
#include "worker.h"
#include "ruledefinitions.h"
#include <QPair>
#include <QStringList>
#include "mydatetime.h"

struct SubRule
{
    // Enumerated values..
    rD::copyMode copymode = rD::noMode;
    rD::fileCompareEntity fileCompareMode = rD::noCompareModeSet;
    rD::fileConditionEntity fieldCondition = rD::nonConditionalMode;

    QPair<int,QString>sizeLimit;
    QPair<QPair<int,QString>,QPair<int,QString>> sizeIntervalLimits;
    QPair<rD::fileCompareEntity,myDateTime>fixedDate;
    QPair<myDateTime,myDateTime>intervalDate;
    bool matchWholeWords = false;
    QStringList keyWords;

    bool operator==(const SubRule other)
    {
        if( copymode != other.copymode ||
                fileCompareMode != other.fileCompareMode ||
                fieldCondition != other.fieldCondition ||
                matchWholeWords != other.matchWholeWords ||
                keyWords != other.keyWords)
        {
            return false;
        }

        if(sizeLimit.first != other.sizeLimit.first ||
                sizeLimit.second != other.sizeLimit.second)
        {
            return false;
        }

        QPair<int,QString> sIntFirst1 = sizeIntervalLimits.first;
        QPair<int,QString> sIntSecond1 = sizeIntervalLimits.second;

        QPair<int,QString> sIntFirst2 = other.sizeIntervalLimits.first;
        QPair<int,QString> sIntSecond2 = other.sizeIntervalLimits.second;

        if(sIntFirst1.first != sIntFirst2.first ||
                sIntFirst1.second != sIntFirst2.second)
        {
            return false;
        }

        if(sIntSecond1.first != sIntSecond2.first ||
                sIntSecond1.second != sIntSecond2.second)
        {
            return false;
        }

        if(fixedDate.first != other.fixedDate.first ||
                fixedDate.second != other.fixedDate.second)
        {
            return false;
        }

        if(intervalDate.first != other.intervalDate.first ||
                intervalDate.second != other.intervalDate.second)
        {
            return false;
        }

        return true;
    }

    bool operator!=(const SubRule other)
    {
        return !operator==(other);
    }
};

struct Rule
{
    QString title = "title";
    rD::fileTypeEntity typeFilter = rD::File;
    rD::fileActionEntity actionRuleEntity;
    QStringList destinationPath;
    QString appliesToPath = "Alle";
    bool deepScanMode = false;
    QList<SubRule> subRules;

    bool operator==(const Rule &other)
    {
        if(title != other.title ||
                typeFilter != other.typeFilter ||
                actionRuleEntity != other.actionRuleEntity ||
                destinationPath != other.destinationPath ||
                appliesToPath != other.appliesToPath ||
                deepScanMode != other.deepScanMode)
        {
            return false;
        }

        for (int i = 0; i < subRules.count(); ++i) {
            SubRule sR1 = subRules.at(i), sR2 = other.subRules.at(i);

            if(sR1 != sR2)
                return false;

        }
        return true;
    }
};


#endif // RULES_H
