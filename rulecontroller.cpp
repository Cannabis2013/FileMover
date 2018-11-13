#include "rulecontroller.h"

ruleController::ruleController()
{}

QString ruleController::mergeStringList(const QStringList strings)
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

QStringList ruleController::splitString(const QString split)
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

QList<QTreeWidgetItem *> ruleController::rulesTreeItems() const
{
    QList<QTreeWidgetItem*>resultingList;
    for(rule r : rules)
    {
        QStringList headerData {r.title,rD::actionToString(r.actionRule),fW::mergeStringList(r.destinationPath)};
        QTreeWidgetItem *pItem = new QTreeWidgetItem(headerData);
        for(subRule sRule : r.subRules)
        {
            QStringList hData;
            hData << rD::fieldConditionalToString(sRule.fieldCondition) << rD::compareModeToString(sRule.fileCompareMode) <<  sRule.Keyword();

            QTreeWidgetItem *cItem = new QTreeWidgetItem(hData);
            pItem->addChild(cItem);
        }
        resultingList << pItem;

    }
    return resultingList;
}

void ruleController::insertRule(const rule r)
{
    rules << r;
}

void ruleController::insertRules(const QList<rule> r)
{
    rules << r;
}

void ruleController::replaceRule(rule r, int index)
{
    rules.replace(index,r);
}

