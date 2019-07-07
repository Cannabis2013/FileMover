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
    rD::fileCompareRuleEntity fileCompareMode = rD::noCompareModeSet;
    Worker::iteratorMode typeMode = Worker::noTypeSet;
    rD::fileConditionRuleEntity fieldCondition = rD::nonConditionalMode;

    QPair<int,QString>sizeLimit;
    QPair<QPair<int,QString>,QPair<int,QString>> sizeIntervalLimits;
    QPair<rD::fileCompareRuleEntity,myDateTime>fixedDate;
    QPair<myDateTime,myDateTime>intervalDate;
    bool matchWholeWords = false;
    QStringList keyWords;
};

struct Rule
{
    QString identification,title = "title";
    rD::fileTypeRuleEntity typeFilter = rD::File;
    rD::fileActionRuleEntity actionRuleEntity;
    QStringList destinationPath;
    QString appliesToPath = "Alle";
    bool deepScanMode = false;
    QList<SubRule> subRules;
};


#endif // RULES_H
