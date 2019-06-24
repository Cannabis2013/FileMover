#ifndef RULES_H
#define RULES_H
#include "worker.h"
#include "entityqueuemanager.h"
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
};

struct Rule
{
    QString identification,title = "title";
    rD::fileActionRule actionRule;
    QStringList destinationPath;
    QString appliesToPath = "Alle";
    bool deepScanMode = false;
    QList<SubRule> subRules;
};


#endif // RULES_H
