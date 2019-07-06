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
    rD::fileComparison fileCompareMode = rD::noCompareModeSet;
    Worker::iteratorMode typeMode = Worker::noTypeSet;
    rD::fileCondition fieldCondition = rD::nonConditionalMode;

    QPair<int,QString>sizeLimit;
    QPair<QPair<int,QString>,QPair<int,QString>> sizeIntervalLimits;
    QPair<rD::fileComparison,myDateTime>fixedDate;
    QPair<myDateTime,myDateTime>intervalDate;
    bool matchWholeWords = false;
    QStringList keyWords;
};

struct Rule
{
    QString identification,title = "title";
    rD::fileTypeFilter typeFilter = rD::Both;
    rD::fileAction actionRule;
    QStringList destinationPath;
    QString appliesToPath = "Alle";
    bool deepScanMode = false;
    QList<SubRule> subRules;
};


#endif // RULES_H
