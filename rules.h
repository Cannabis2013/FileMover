#ifndef RULES_H
#define RULES_H
#include "worker.h"
#include "ruledefinitions.h"
#include <QPair>
#include <QStringList>
#include "mydatetime.h"

typedef QPair<uint,QString> IntervalUnit;
typedef QPair<IntervalUnit,IntervalUnit> SizeLimits;

struct SubRule
{
    // Enumerated values..
    rD::copyMode copymode = rD::noMode;
    rD::ruleCompareCriteria compareCriteria = rD::noCompareModeSet;
    rD::ruleCriteria criteria = rD::nonConditionalMode;

    QPair<uint,QString>sizeLimit;
    SizeLimits sizeInterval;
    myDateTime date;
    QPair<myDateTime,myDateTime>dateIntervals;
    bool matchWholeWords = false;
    QStringList keyWords;
};

typedef QPair<int,QString> SizeOperand;
typedef QPair<QPair<int,QString>,QPair<int,QString>> SizeInterval;

struct Rule
{
    QString title = "title";
    rD::fileTypeEntity typeFilter = rD::File;
    rD::ruleAction actionRuleEntity;
    QStringList destinationPaths;
    QString appliesToPath = "Alle";
    bool deepScanMode = false;
    QList<SubRule> subRules;
};


#endif // RULES_H
