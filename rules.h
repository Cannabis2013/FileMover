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
};

struct Rule
{
    QString identification,title = "title";
    rD::fileTypeEntity typeFilter = rD::File;
    rD::fileActionEntity actionRuleEntity;
    QStringList destinationPath;
    QString appliesToPath = "Alle";
    bool deepScanMode = false;
    QList<SubRule> subRules;
};


#endif // RULES_H
