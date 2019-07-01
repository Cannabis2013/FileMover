#include "rulesmanager.h"

rulesManager::rulesManager(QString appName, QString orgName):
    AbstractPersistence (appName,orgName)
{
    readSettings();
}

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

QString rulesManager::ruleKeyWordToString(SubRule sRule)
{
    if(sRule.fieldCondition == rD::sizeMode &&
            sRule.fileCompareMode != rD::interval)
        return QString::number(sRule.sizeLimit.first) + " " + sRule.sizeLimit.second;
    else if(sRule.fieldCondition == rD::sizeMode &&
            sRule.fileCompareMode == rD::interval)
        return rulesManager::ruleSizeLimitsToString(sRule);
    else if((sRule.fieldCondition == rD::dateCreatedMode || sRule.fieldCondition == rD::dateModifiedMode) &&
            sRule.fileCompareMode != rD::interval)
        return sRule.fixedDate.second.toString("dd.MM.yyyy");
    else if((sRule.fieldCondition == rD::dateCreatedMode || sRule.fieldCondition == rD::dateModifiedMode) &&
            sRule.fileCompareMode == rD::interval)
        return rulesManager::ruleDateLimitsToString(sRule);
    else if(sRule.fieldCondition == rD::typeMode)
        return rD::typeFromEnum(sRule.typeMode);
    else
        return Worker::mergeStringList(sRule.keyWords);
}


QString rulesManager::ruleSizeLimitsToString(SubRule sRule)
{
    QString minSize = QString::number(sRule.sizeIntervalLimits.first.first),
            maxSize = QString::number(sRule.sizeIntervalLimits.second.first);
    QString sizeUnitMin = sRule.sizeIntervalLimits.first.second,
            sizeUnitMax = sRule.sizeIntervalLimits.second.second;
    return "Min: " + minSize + " " + sizeUnitMin
            + " " + "max: " + maxSize + " " + sizeUnitMax;
}

QString rulesManager::ruleDateLimitsToString(SubRule sRule)
{
    QString startDate = sRule.intervalDate.first.date().toString("dd.MM.yyyy"),
            endDate = sRule.intervalDate.second.date().toString("dd.MM.yyyy");
    return "Start dato: " + startDate + " slut dato: " + endDate;
}

QList<QTreeWidgetItem *> rulesManager::ruleItems() const
{
    rD rDefs;
    QList<QTreeWidgetItem*>resultingList;
    for(Rule r : rules)
    {
        QStringList headerData {r.title,rDefs.actionToString(r.actionRule),Worker::mergeStringList(r.destinationPath)};
        QTreeWidgetItem *pItem = new QTreeWidgetItem(headerData);
        QIcon itemIcon = QIcon(":/My Images/Ressources/rule_icon.png");
        pItem->setIcon(0,itemIcon);
        for(SubRule sRule : r.subRules)
        {
            QStringList hData;
            hData << rDefs.fieldConditionToString(sRule.fieldCondition) <<
                     rDefs.compareToString(sRule.fileCompareMode) <<
                     rulesManager::ruleKeyWordToString(sRule);

            QTreeWidgetItem *cItem = new QTreeWidgetItem(hData);
            cItem->setIcon(0,QIcon(":/My Images/Ressources/sub_rule_icon.png"));
            for (int i = 0; i < hData.count(); ++i)
                cItem->setFont(i,QApplication::font());
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
    int total = persistenceSettings->beginReadArray("Rules");
    for (int i = 0; i < total; ++i)
    {
        Rule r;
        persistenceSettings->setArrayIndex(i);
        r.title = persistenceSettings->value("Title","Title").toString();
        r.actionRule = static_cast<rD::fileAction>(persistenceSettings->value("Action","").toInt());
        r.appliesToPath = persistenceSettings->value("ApplyPath","Alle").toString();
        r.destinationPath = Worker::splitString(persistenceSettings->value("Destination paths","").toString());
        r.deepScanMode = persistenceSettings->value("Scan Mode",false).toBool();
        int count = persistenceSettings->beginReadArray("Subrules");
        for (int n = 0; n < count; ++n)
        {
            SubRule sRule;
            persistenceSettings->setArrayIndex(n);

            sRule.copymode = static_cast<rD::copyMode>(persistenceSettings->value("Copymode",0).toInt());
            sRule.fieldCondition = static_cast<rD::fileCondition>(persistenceSettings->value("Condition","").toInt());
            sRule.fileCompareMode = static_cast<rD::fileComparison>(persistenceSettings->value("Comparemode",0).toInt());

            sRule.matchWholeWords = persistenceSettings->value("Matchwholewords",false).toBool();
            sRule.keyWords = Worker::splitString(persistenceSettings->value("Keywords","").toString());

            sRule.sizeLimit.first = persistenceSettings->value("Sizelimit",0).toInt();
            sRule.sizeLimit.second = persistenceSettings->value("Sizelimitunit","kb").toString();

            persistenceSettings->beginGroup("Sizelimits");
            sRule.sizeIntervalLimits.first.first = persistenceSettings->value("Minsizeinterval",0).toInt();
            sRule.sizeIntervalLimits.first.second = persistenceSettings->value("Minsizeunitinterval","kb").toString();
            sRule.sizeIntervalLimits.second.first = persistenceSettings->value("Maxsizeinterval",0).toInt();
            sRule.sizeIntervalLimits.second.second = persistenceSettings->value("Maxsizeunitinterval","kb").toString();
            persistenceSettings->endGroup();

            sRule.fixedDate.first = static_cast<rD::fileComparison>(persistenceSettings->value("Comparemode",0).toInt());
            sRule.fixedDate.second = QDateTime::fromString(persistenceSettings->value("Datetime","").toString(),"dd.MM.yyyy");

            persistenceSettings->beginGroup("Datelimits");
            sRule.intervalDate.first = myDateTime::fromString(persistenceSettings->value("Startdate","01.01.2000").toString());
            sRule.intervalDate.second = myDateTime::fromString(persistenceSettings->value("Enddate","01.01.2000").toString());
            persistenceSettings->endGroup();

            sRule.typeMode = static_cast<Worker::iteratorMode>(persistenceSettings->value("Iteratormode",0).toInt());

            r.subRules.append(sRule);
        }
        persistenceSettings->endArray();
        rules << r;
    }
    persistenceSettings->endArray();
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
    emit stateChanged();
}

void rulesManager::replaceRule(const Rule r, QString title)
{
    for (int i = 0; i < rules.count(); ++i) {
        Rule currentRule = rules.at(i);
        if(currentRule.title == title)
        {
            rules.replace(i,r);
            emit stateChanged();
            return;
        }
    }
    throw QString("Item not found");
}

void rulesManager::swapRule(int i, int j)
{
    if(i >= rules.count() || j >= rules.count())
        throw std::out_of_range("Index out of range");
    if(i < 0 || j < 0)
        throw std::out_of_range("Index out of range");

    rules.swap(i,j);

    emit stateChanged();
}

