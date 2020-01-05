#include "rulesmanager.h"

rulesManager::rulesManager(const QString &appName, const QString &orgName):
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
    if(sRule.fieldCondition == rD::fileSize &&
            sRule.fileCompareMode != rD::interval)
        return QString::number(sRule.sizeLimit.first) + " " + sRule.sizeLimit.second;
    else if(sRule.fieldCondition == rD::fileSize &&
            sRule.fileCompareMode == rD::interval)
        return rulesManager::ruleSizeLimitsToString(sRule);
    else if((sRule.fieldCondition == rD::fileCreatedMode || sRule.fieldCondition == rD::fileModifiedMode) &&
            sRule.fileCompareMode != rD::interval)
        return sRule.date.toString("dd.MM.yyyy");
    else if((sRule.fieldCondition == rD::fileCreatedMode || sRule.fieldCondition == rD::fileModifiedMode) &&
            sRule.fileCompareMode == rD::interval)
        return rulesManager::ruleDateLimitsToString(sRule);
    else
        return Worker::mergeStringList(sRule.keyWords);
}


QString rulesManager::ruleSizeLimitsToString(SubRule sRule)
{
    QString minSize = QString::number(sRule.sizeInterval.first.first),
            maxSize = QString::number(sRule.sizeInterval.second.first);
    QString sizeUnitMin = sRule.sizeInterval.first.second,
            sizeUnitMax = sRule.sizeInterval.second.second;
    return "Min: " + minSize + " " + sizeUnitMin
            + " " + "max: " + maxSize + " " + sizeUnitMax;
}

QString rulesManager::ruleDateLimitsToString(SubRule sRule)
{
    QString startDate = sRule.dateIntervals.first.date().toString("dd.MM.yyyy"),
            endDate = sRule.dateIntervals.second.date().toString("dd.MM.yyyy");
    return "Start dato: " + startDate + " slut dato: " + endDate;
}

QList<QTreeWidgetItem *> rulesManager::ruleItems() const
{
    rD rDefs;
    QList<QTreeWidgetItem*>resultingList;
    for(Rule r : rules)
    {
        QStringList headerData {r.title,rDefs.fileActionEntityToString(r.actionRuleEntity),Worker::mergeStringList(r.destinationPath)};
        QTreeWidgetItem *pItem = new QTreeWidgetItem(headerData);
        QIcon itemIcon = QIcon(":/My Images/Ressources/rule_icon.png");
        pItem->setIcon(0,itemIcon);
        for(SubRule sRule : r.subRules)
        {
            QStringList hData;
            hData << rDefs.fileConditionEntityToString(sRule.fieldCondition) <<
                     rDefs.fileCompareEntityToString(sRule.fileCompareMode) <<
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

const Rule rulesManager::rule(QString title)
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
        r.actionRuleEntity = static_cast<rD::fileActionEntity>(persistenceSettings->value("Action","").toInt());
        r.appliesToPath = persistenceSettings->value("ApplyPath","Alle").toString();
        r.typeFilter = static_cast<rD::fileTypeEntity>(persistenceSettings->value("Scan type filter","").toInt());
        r.destinationPath = Worker::splitString(persistenceSettings->value("Destination paths","").toString());
        int count = persistenceSettings->beginReadArray("Subrules");
        for (int j = 0; j < count; ++j)
        {
            SubRule sRule;
            persistenceSettings->setArrayIndex(j);

            sRule.copymode = static_cast<rD::copyMode>(persistenceSettings->value("Copymode",0).toInt());
            sRule.fieldCondition = static_cast<rD::fileConditionEntity>(persistenceSettings->value("Condition","").toInt());
            sRule.fileCompareMode = static_cast<rD::fileCompareEntity>(persistenceSettings->value("Comparemode",0).toInt());

            sRule.matchWholeWords = persistenceSettings->value("Matchwholewords",false).toBool();
            sRule.keyWords = Worker::splitString(persistenceSettings->value("Keywords","").toString());

            sRule.sizeLimit.first = persistenceSettings->value("Sizelimit",0).toInt();
            sRule.sizeLimit.second = persistenceSettings->value("Sizelimitunit","kb").toString();

            persistenceSettings->beginGroup("Sizelimits");
            sRule.sizeInterval.first.first = persistenceSettings->value("Minsizeinterval",0).toInt();
            sRule.sizeInterval.first.second = persistenceSettings->value("Minsizeunitinterval","kb").toString();
            sRule.sizeInterval.second.first = persistenceSettings->value("Maxsizeinterval",0).toInt();
            sRule.sizeInterval.second.second = persistenceSettings->value("Maxsizeunitinterval","kb").toString();
            persistenceSettings->endGroup();
            QDateTime persistedDate = QDateTime::fromString(persistenceSettings->value("Datetime","").toString(),"dd.MM.yyyy");
            sRule.date = static_cast<myDateTime>(persistedDate);

            persistenceSettings->beginGroup("Datelimits");

            QDateTime persistedStartDate = myDateTime::fromString(persistenceSettings->value("Startdate","01.01.2000").toString());
            QDateTime persistedEndDate = myDateTime::fromString(persistenceSettings->value("Enddate","01.01.2000").toString());

            sRule.dateIntervals.first = static_cast<myDateTime>(persistedStartDate);
            sRule.dateIntervals.second = static_cast<myDateTime>(persistedEndDate);

            persistenceSettings->endGroup();

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
        persistenceSettings->setValue("Action",r.actionRuleEntity);
        persistenceSettings->setValue("Scan type filter",r.typeFilter);
        persistenceSettings->setValue("ApplyPath",r.appliesToPath);
        persistenceSettings->setValue("Destination paths",
                   mergeStringList(r.destinationPath));
        persistenceSettings->setValue("Scan Mode",r.deepScanMode);
        QList<SubRule>sRules = r.subRules;
        int total = sRules.count();
        persistenceSettings->beginWriteArray("Subrules",total);
        for (int j = 0; j < total; ++j)
        {
            SubRule sRule = sRules.at(j);
            persistenceSettings->setArrayIndex(j);

            persistenceSettings->setValue("Copymode",sRule.copymode);
            persistenceSettings->setValue("Condition",sRule.fieldCondition);
            persistenceSettings->setValue("Comparemode",sRule.fileCompareMode);

            persistenceSettings->setValue("Matchwholewords",sRule.matchWholeWords);
            persistenceSettings->setValue("Keywords",Worker::mergeStringList(sRule.keyWords));

            persistenceSettings->setValue("Sizelimit",sRule.sizeLimit.first);
            persistenceSettings->setValue("Sizelimitunit",sRule.sizeLimit.second);

            persistenceSettings->beginGroup("Sizelimits");
            persistenceSettings->setValue("Minsizeinterval",sRule.sizeInterval.first.first);
            persistenceSettings->setValue("Minsizeunitinterval",sRule.sizeInterval.first.second);
            persistenceSettings->setValue("Maxsizeinterval",sRule.sizeInterval.second.first);
            persistenceSettings->setValue("Maxsizeunitinterval",sRule.sizeInterval.second.second);
            persistenceSettings->endGroup();

            persistenceSettings->setValue("Datetime",sRule.date.toString("dd.MM.yyyy"));

            persistenceSettings->beginGroup("Datelimits");
            persistenceSettings->setValue("Startdate",sRule.dateIntervals.first.toString("dd.MM.yyyy"));
            persistenceSettings->setValue("Enddate",sRule.dateIntervals.second.toString("dd.MM.yyyy"));
            persistenceSettings->endGroup();
        }
        persistenceSettings->endArray();
    }
    persistenceSettings->endArray();
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
        throw new std::out_of_range("Index out of range");
    if(i < 0 || j < 0)
        throw new std::out_of_range("Index out of range");

    rules.swapItemsAt(i,j);

    emit stateChanged();
}

