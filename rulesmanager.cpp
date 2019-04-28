#include "rulesmanager.h"

rulesManager::rulesManager()
{}

QString rulesManager::mergeStringList(const QStringList strings)
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

QStringList rulesManager::splitString(const QString split)
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

QList<QTreeWidgetItem *> rulesManager::rulesTreeItems() const
{
    rD rDefs;
    QList<QTreeWidgetItem*>resultingList;
    for(rule r : rules)
    {
        QStringList headerData {r.title,rDefs.actionToString(r.actionRule),fW::mergeStringList(r.destinationPath)};
        QTreeWidgetItem *pItem = new QTreeWidgetItem(headerData);
        for(subRule sRule : r.subRules)
        {
            QStringList hData;
            hData << rDefs.fieldConditionToString(sRule.fieldCondition) << rDefs.compareToString(sRule.fileCompareMode) <<  sRule.Keyword();

            QTreeWidgetItem *cItem = new QTreeWidgetItem(hData);
            pItem->addChild(cItem);
        }
        resultingList << pItem;

    }
    return resultingList;
}

void rulesManager::insertRule(const rule r)
{
    rules << r;
}

void rulesManager::insertRules(const QList<rule> r)
{
    rules << r;
}

void rulesManager::replaceRule(rule r, int index)
{
    rules.replace(index,r);
}

