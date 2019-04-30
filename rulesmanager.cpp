#include "rulesmanager.h"

rulesManager::rulesManager(QString appName, QString orgName):
    AbstractPersistence (appName,orgName)
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
    for(Rule r : rules)
    {
        QStringList headerData {r.title,rDefs.actionToString(r.actionRule),fW::mergeStringList(r.destinationPath)};
        QTreeWidgetItem *pItem = new QTreeWidgetItem(headerData);
        for(SubRule sRule : r.subRules)
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

void rulesManager::addRule(const Rule r)
{
    rules << r;
}

void rulesManager::addRules(const QList<Rule> r)
{
    rules << r;
}

void rulesManager::removeRule(const QString title)
{
    for (int i = 0; i < rules.count(); ++i) {
        Rule r = rules.at(i);
        if(r.title == title)
        {
            rules.removeAt(i);
            return;
        }
    }
    throw QString("Item not found.");
}

Rule rulesManager::rule(QString title)
{
    for(Rule rule : rules)
    {
        if(rule.title == title)
            return Rule(rule);
    }

    return Rule();
}

void rulesManager::readSettings()
{
    QList<Rule>rules;
    QSettings s;
    int total = s.beginReadArray("Rules");
    for (int i = 0; i < total; ++i)
    {
        Rule r;
        s.setArrayIndex(i);
        r.title = s.value("Title","Title").toString();
        r.actionRule = static_cast<rD::fileActionRule>(s.value("Action","").toInt());
        r.appliesToPath = s.value("ApplyPath","Alle").toString();
        r.destinationPath = Worker::splitString(s.value("Destination paths","").toString());
        r.deepScanMode = s.value("Scan Mode",false).toBool();
        int count = s.beginReadArray("Subrules");
        for (int n = 0; n < count; ++n)
        {
            SubRule sRule;
            s.setArrayIndex(n);

            sRule.copymode = static_cast<rD::copyMode>(s.value("Copymode",0).toInt());
            sRule.fieldCondition = static_cast<rD::fileFieldCondition>(s.value("Condition","").toInt());
            sRule.fileCompareMode = static_cast<rD::compareMode>(s.value("Comparemode",0).toInt());

            sRule.matchWholeWords = s.value("Matchwholewords",false).toBool();
            sRule.keyWords = Worker::splitString(s.value("Keywords","").toString());

            sRule.sizeLimit.first = s.value("Sizelimit",0).toInt();
            sRule.sizeLimit.second = s.value("Sizelimitunit","kb").toString();

            s.beginGroup("Sizelimits");
            sRule.sizeIntervalLimits.first.first = s.value("Minsizeinterval",0).toInt();
            sRule.sizeIntervalLimits.first.second = s.value("Minsizeunitinterval","kb").toString();
            sRule.sizeIntervalLimits.second.first = s.value("Maxsizeinterval",0).toInt();
            sRule.sizeIntervalLimits.second.second = s.value("Maxsizeunitinterval","kb").toString();
            s.endGroup();

            sRule.fixedDate.first = static_cast<rD::compareMode>(s.value("Comparemode",0).toInt());
            sRule.fixedDate.second = QDateTime::fromString(s.value("Datetime","").toString(),"dd.MM.yyyy");

            s.beginGroup("Datelimits");
            sRule.intervalDate.first = myDateTime::fromString(s.value("Startdate","01.01.2000").toString());
            sRule.intervalDate.second = myDateTime::fromString(s.value("Enddate","01.01.2000").toString());
            s.endGroup();

            sRule.typeMode = static_cast<wrk::iteratorMode>(s.value("Iteratormode",0).toInt());

            r.subRules.append(sRule);
        }
        s.endArray();
        rules << r;
    }
    s.endArray();
    addRules(rules);
}

void rulesManager::writeSettings()
{
    QList<Rule> rules = ruleslist();

    QSettings s;
    s.remove("Rules");
    s.beginWriteArray("Rules",rules.count());
    for (int i = 0; i < rules.count(); ++i)
    {
        s.setArrayIndex(i);
        Rule r = rules.at(i);
        s.setValue("Title",r.title);
        s.setValue("Action",r.actionRule);
        s.setValue("ApplyPath",r.appliesToPath);
        s.setValue("Destination paths",
                   mergeStringList(r.destinationPath));
        s.setValue("Scan Mode",r.deepScanMode);
        QList<SubRule>sRules = r.subRules;
        int total = sRules.count();
        s.beginWriteArray("Subrules",total);
        for (int n = 0; n < total; ++n)
        {
            SubRule sRule = sRules.at(n);
            s.setArrayIndex(n);

            s.setValue("Copymode",sRule.copymode);
            s.setValue("Condition",sRule.fieldCondition);
            s.setValue("Comparemode",sRule.fileCompareMode);

            s.setValue("Matchwholewords",sRule.matchWholeWords);
            s.setValue("Keywords",Worker::mergeStringList(sRule.keyWords));

            s.setValue("Sizelimit",sRule.sizeLimit.first);
            s.setValue("Sizelimitunit",sRule.sizeLimit.second);

            s.beginGroup("Sizelimits");
            s.setValue("Minsizeinterval",sRule.sizeIntervalLimits.first.first);
            s.setValue("Minsizeunitinterval",sRule.sizeIntervalLimits.first.second);
            s.setValue("Maxsizeinterval",sRule.sizeIntervalLimits.second.first);
            s.setValue("Maxsizeunitinterval",sRule.sizeIntervalLimits.second.second);
            s.endGroup();

            s.setValue("Datetime",sRule.fixedDate.second.toString("dd.MM.yyyy"));

            s.beginGroup("Datelimits");
            s.setValue("Startdate",sRule.intervalDate.first.toString("dd.MM.yyyy"));
            s.setValue("Enddate",sRule.intervalDate.second.toString("dd.MM.yyyy"));
            s.endGroup();

            s.setValue("Iteratormode",sRule.typeMode);
        }
        s.endArray();
    }
    s.endArray();
}

void rulesManager::writeRulesToReg()
{

}

void rulesManager::replaceRule(const Rule r, int index)
{
    rules.replace(index,r);
}

void rulesManager::replaceRule(const Rule r, QString title)
{
    for (int i = 0; i < rules.count(); ++i) {
        Rule r = rules.at(i);
        if(r.title == title)
        {
            rules.replace(i,r);
            return;
        }
    }
    throw QString("Item not found");
}

