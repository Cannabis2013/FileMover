#include "rulesmanager.h"

rulesManager::rulesManager(QString appName, QString orgName):
    AbstractPersistence (appName,orgName)
{}

rulesManager::~rulesManager()
{
    writeSettings();
}

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

QList<QTreeWidgetItem *> rulesManager::ruleItems() const
{
    rD rDefs;
    QList<QTreeWidgetItem*>resultingList;
    for(Rule r : rules)
    {
        QStringList headerData {r.title,rDefs.actionToString(r.actionRule),Worker::mergeStringList(r.destinationPath)};
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
    emit stateChanged();
}

void rulesManager::addRules(const QList<Rule> r)
{
    rules << r;
    emit stateChanged();
}

void rulesManager::removeRule(const QString title)
{
    for (int i = 0; i < rules.count(); ++i) {
        Rule r = rules.at(i);
        if(r.title == title)
        {
            rules.removeAt(i);
            emit stateChanged();
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

            sRule.typeMode = static_cast<Worker::iteratorMode>(s.value("Iteratormode",0).toInt());

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

    persistenceSettings->remove("Rules");
    persistenceSettings->beginWriteArray("Rules",rules.count());
    for (int i = 0; i < rules.count(); ++i)
    {
        persistenceSettings->setArrayIndex(i);
        Rule r = rules.at(i);
        persistenceSettings->setValue("Title",r.title);
        persistenceSettings->setValue("Action",r.actionRule);
        persistenceSettings->setValue("ApplyPath",r.appliesToPath);
        persistenceSettings->setValue("Destination paths",
                   mergeStringList(r.destinationPath));
        persistenceSettings->setValue("Scan Mode",r.deepScanMode);
        QList<SubRule>sRules = r.subRules;
        int total = sRules.count();
        persistenceSettings->beginWriteArray("Subrules",total);
        for (int n = 0; n < total; ++n)
        {
            SubRule sRule = sRules.at(n);
            persistenceSettings->setArrayIndex(n);

            persistenceSettings->setValue("Copymode",sRule.copymode);
            persistenceSettings->setValue("Condition",sRule.fieldCondition);
            persistenceSettings->setValue("Comparemode",sRule.fileCompareMode);

            persistenceSettings->setValue("Matchwholewords",sRule.matchWholeWords);
            persistenceSettings->setValue("Keywords",Worker::mergeStringList(sRule.keyWords));

            persistenceSettings->setValue("Sizelimit",sRule.sizeLimit.first);
            persistenceSettings->setValue("Sizelimitunit",sRule.sizeLimit.second);

            persistenceSettings->beginGroup("Sizelimits");
            persistenceSettings->setValue("Minsizeinterval",sRule.sizeIntervalLimits.first.first);
            persistenceSettings->setValue("Minsizeunitinterval",sRule.sizeIntervalLimits.first.second);
            persistenceSettings->setValue("Maxsizeinterval",sRule.sizeIntervalLimits.second.first);
            persistenceSettings->setValue("Maxsizeunitinterval",sRule.sizeIntervalLimits.second.second);
            persistenceSettings->endGroup();

            persistenceSettings->setValue("Datetime",sRule.fixedDate.second.toString("dd.MM.yyyy"));

            persistenceSettings->beginGroup("Datelimits");
            persistenceSettings->setValue("Startdate",sRule.intervalDate.first.toString("dd.MM.yyyy"));
            persistenceSettings->setValue("Enddate",sRule.intervalDate.second.toString("dd.MM.yyyy"));
            persistenceSettings->endGroup();

            persistenceSettings->setValue("Iteratormode",sRule.typeMode);
        }
        persistenceSettings->endArray();
    }
    persistenceSettings->endArray();
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

void rulesManager::swapRule(int i, int j)
{
    if(i >= rules.count() || j >= rules.count())
        throw new std::overflow_error("");
    if(i < 0 || j < 0)
        throw new std::overflow_error("");

    rules.swap(i,j);
}

