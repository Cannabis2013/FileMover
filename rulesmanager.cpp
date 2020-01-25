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
        QStringList headerData {r.title,rDefs.fileActionEntityToString(r.actionRuleEntity),Worker::mergeStringList(r.destinationPaths)};
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
    QSettings *pSettings = persistenceSettings();
    int total = pSettings->beginReadArray("Rules");
    for (int i = 0; i < total; ++i)
    {
        Rule r;
        pSettings->setArrayIndex(i);
        r.title = pSettings->value("Title","Title").toString();
        r.actionRuleEntity = static_cast<rD::ruleAction>(pSettings->value("Action","").toInt());
        r.appliesToPath = pSettings->value("ApplyPath","Alle").toString();
        r.typeFilter = static_cast<rD::fileTypeEntity>(pSettings->value("Scan type filter","").toInt());
        r.destinationPaths = Worker::splitString(pSettings->value("Destination paths","").toString());
        int count = pSettings->beginReadArray("Subrules");
        for (int j = 0; j < count; ++j)
        {
            SubRule sRule;
            pSettings->setArrayIndex(j);

            sRule.copymode = static_cast<rD::copyMode>(pSettings->value("Copymode",0).toInt());
            sRule.fieldCondition = static_cast<rD::ruleCriteria>(pSettings->value("Condition","").toInt());
            sRule.fileCompareMode = static_cast<rD::ruleCompareCriteria>(pSettings->value("Comparemode",0).toInt());

            sRule.matchWholeWords = pSettings->value("Matchwholewords",false).toBool();
            sRule.keyWords = Worker::splitString(pSettings->value("Keywords","").toString());

            sRule.sizeLimit.first = pSettings->value("Sizelimit",0).toInt();
            sRule.sizeLimit.second = pSettings->value("Sizelimitunit","kb").toString();

            pSettings->beginGroup("Sizelimits");
            sRule.sizeInterval.first.first = pSettings->value("Minsizeinterval",0).toInt();
            sRule.sizeInterval.first.second = pSettings->value("Minsizeunitinterval","kb").toString();
            sRule.sizeInterval.second.first = pSettings->value("Maxsizeinterval",0).toInt();
            sRule.sizeInterval.second.second = pSettings->value("Maxsizeunitinterval","kb").toString();
            pSettings->endGroup();
            QDateTime persistedDate = QDateTime::fromString(pSettings->value("Datetime","").toString(),"dd.MM.yyyy");
            sRule.date = static_cast<myDateTime>(persistedDate);

            pSettings->beginGroup("Datelimits");

            QDateTime persistedStartDate = myDateTime::fromString(pSettings->value("Startdate","01.01.2000").toString());
            QDateTime persistedEndDate = myDateTime::fromString(pSettings->value("Enddate","01.01.2000").toString());

            sRule.dateIntervals.first = static_cast<myDateTime>(persistedStartDate);
            sRule.dateIntervals.second = static_cast<myDateTime>(persistedEndDate);

            pSettings->endGroup();

            r.subRules.append(sRule);
        }
        pSettings->endArray();
        rules << r;
    }
    pSettings->endArray();
    addRules(rules);
}

void rulesManager::writeSettings()
{
    QList<Rule> rules = ruleslist();
    QSettings *pSettings = persistenceSettings();
    pSettings->remove("Rules");
    pSettings->beginWriteArray("Rules",rules.count());
    for (int i = 0; i < rules.count(); ++i)
    {
        pSettings->setArrayIndex(i);
        Rule r = rules.at(i);
        pSettings->setValue("Title",r.title);
        pSettings->setValue("Action",r.actionRuleEntity);
        pSettings->setValue("Scan type filter",r.typeFilter);
        pSettings->setValue("ApplyPath",r.appliesToPath);
        pSettings->setValue("Destination paths",
                   mergeStringList(r.destinationPaths));
        pSettings->setValue("Scan Mode",r.deepScanMode);
        QList<SubRule>sRules = r.subRules;
        int total = sRules.count();
        pSettings->beginWriteArray("Subrules",total);
        for (int j = 0; j < total; ++j)
        {
            SubRule sRule = sRules.at(j);
            pSettings->setArrayIndex(j);

            pSettings->setValue("Copymode",sRule.copymode);
            pSettings->setValue("Condition",sRule.fieldCondition);
            pSettings->setValue("Comparemode",sRule.fileCompareMode);

            pSettings->setValue("Matchwholewords",sRule.matchWholeWords);
            pSettings->setValue("Keywords",Worker::mergeStringList(sRule.keyWords));

            pSettings->setValue("Sizelimit",sRule.sizeLimit.first);
            pSettings->setValue("Sizelimitunit",sRule.sizeLimit.second);

            pSettings->beginGroup("Sizelimits");
            pSettings->setValue("Minsizeinterval",sRule.sizeInterval.first.first);
            pSettings->setValue("Minsizeunitinterval",sRule.sizeInterval.first.second);
            pSettings->setValue("Maxsizeinterval",sRule.sizeInterval.second.first);
            pSettings->setValue("Maxsizeunitinterval",sRule.sizeInterval.second.second);
            pSettings->endGroup();

            pSettings->setValue("Datetime",sRule.date.toString("dd.MM.yyyy"));

            pSettings->beginGroup("Datelimits");
            pSettings->setValue("Startdate",sRule.dateIntervals.first.toString("dd.MM.yyyy"));
            pSettings->setValue("Enddate",sRule.dateIntervals.second.toString("dd.MM.yyyy"));
            pSettings->endGroup();
        }
        pSettings->endArray();
    }
    pSettings->endArray();
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

