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

QList<QTreeWidgetItem *> rulesManager::ruleItems() const
{
    RuleDefinitions rDefs;
    QList<QTreeWidgetItem*>resultingList;
    for(Rule r : rules)
    {
        QStringList headerData {r.title(),rDefs.fileActionEntityToString(r.actionRuleEntity()),
                    StaticStringCollections::mergeStringList(r.destinationPaths())};
        QTreeWidgetItem *pItem = new QTreeWidgetItem(headerData);
        QIcon itemIcon = QIcon(":/My Images/Ressources/rule_icon.png");
        pItem->setIcon(0,itemIcon);
        for(SubRule sRule : r.subRules())
        {
            QStringList hData;
            hData << rDefs.buildStringFromCriteria(sRule.criteria()) <<
                     rDefs.buildStringFromCompareCriteria(sRule.compareCriteria()) <<
                     StaticStringCollections::ruleKeyWordToString(sRule);

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
        if(r.title() == title)
        {
            rules.removeAt(i);
            emit stateChanged();
            return;
        }
    }
    throw QString("Item not found.");
}

const Rule rulesManager::rule(QString title) const
{
    for(Rule rule : rules)
    {
        if(rule.title() == title)
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
        pSettings->setArrayIndex(i);
        auto title = pSettings->value("Title","Title").toString();
        auto action = static_cast<RRT::RuleAction>(pSettings->value("Action","").toInt());
        auto appliesTo = pSettings->value("ApplyPath","Alle").toString();
        auto type = static_cast<RRT::FileTypeEntity>(pSettings->value("Scan type filter","").toInt());
        auto destinations = StaticStringCollections::splitString(pSettings->value("Destination paths","").toString());
        int count = pSettings->beginReadArray("Subrules");
        auto r = RuleBuilder::buildOrdinaryRule(title,appliesTo,destinations,action,type);
        for (int j = 0; j < count; ++j)
        {
            pSettings->setArrayIndex(j);

            //auto mode = static_cast<RRT::CopyMode>(pSettings->value("Copymode",0).toInt());
            auto criteria = static_cast<RRT::RuleCriteria>(pSettings->value("Condition","").toInt());
            auto compareCriteria = static_cast<RRT::RuleCompareCriteria>(pSettings->value("Comparemode",0).toInt());

            auto matchWholeWords = pSettings->value("Matchwholewords",false).toBool();
            auto keyWords = StaticStringCollections::splitString(pSettings->value("Keywords","").toString());

            auto sLimit = pSettings->value("Sizelimit",0).toInt();
            auto sizeUnit = pSettings->value("Sizelimitunit","kb").toString();
            auto sizeLimit = SizeLimit(sLimit,sizeUnit);

            pSettings->beginGroup("Sizelimits");

            auto lowerSizeLimit = pSettings->value("Minsizeinterval",0).toInt();
            auto lowerSizeUnit = pSettings->value("Minsizeunitinterval","kb").toString();
            auto upperSizeLimit = pSettings->value("Maxsizeinterval",0).toInt();
            auto upperSizeUnit = pSettings->value("Maxsizeunitinterval","kb").toString();
            auto lowerInterval = SizeLimit(lowerSizeLimit,lowerSizeUnit);
            auto upperInterval = SizeLimit(upperSizeLimit,upperSizeUnit);
            auto sizeLimits = SizeLimits(lowerInterval,upperInterval);

            pSettings->endGroup();
            QDateTime persistedDate = QDateTime::fromString(pSettings->value("Datetime","").toString(),"dd.MM.yyyy");
            auto date = static_cast<myDateTime>(persistedDate);

            pSettings->beginGroup("Datelimits");

            QDateTime persistedStartDate = myDateTime::fromString(pSettings->value("Startdate","01.01.2000").toString());
            QDateTime persistedEndDate = myDateTime::fromString(pSettings->value("Enddate","01.01.2000").toString());

            auto lowerDate = static_cast<myDateTime>(persistedStartDate);
            auto upperDate = static_cast<myDateTime>(persistedEndDate);
            auto dates = DateInterval(lowerDate,upperDate);
            pSettings->endGroup();

            auto sRule = RuleBuilder::buildSubRule(criteria,
                                                   compareCriteria,
                                                   keyWords,
                                                   sizeLimit,
                                                   date,
                                                   sizeLimits,
                                                   dates,
                                                   matchWholeWords);

            RuleBuilder::attachCriteria(sRule,r);

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
        pSettings->setValue("Title",r.title());
        pSettings->setValue("Action",r.actionRuleEntity());
        pSettings->setValue("Scan type filter",r.typeFilter());
        pSettings->setValue("ApplyPath",r.appliesToPath());
        pSettings->setValue("Destination paths",
                   StaticStringCollections::mergeStringList(r.destinationPaths()));
        pSettings->setValue("Scan Mode",r.deepScanMode());
        QList<SubRule>sRules = r.subRules();
        int total = sRules.count();
        pSettings->beginWriteArray("Subrules",total);
        for (int j = 0; j < total; ++j)
        {
            SubRule sRule = sRules.at(j);
            pSettings->setArrayIndex(j);

            pSettings->setValue("Copymode",sRule.copyMode());
            pSettings->setValue("Condition",sRule.criteria());
            pSettings->setValue("Comparemode",sRule.compareCriteria());

            pSettings->setValue("Matchwholewords",sRule.matchWholeWords());
            pSettings->setValue("Keywords",StaticStringCollections::mergeStringList(sRule.keyWords()));

            pSettings->setValue("Sizelimit",sRule.sizeLimit().first);
            pSettings->setValue("Sizelimitunit",sRule.sizeLimit().second);

            pSettings->beginGroup("Sizelimits");
            pSettings->setValue("Minsizeinterval",sRule.sizeInterval().first.first);
            pSettings->setValue("Minsizeunitinterval",sRule.sizeInterval().first.second);
            pSettings->setValue("Maxsizeinterval",sRule.sizeInterval().second.first);
            pSettings->setValue("Maxsizeunitinterval",sRule.sizeInterval().second.second);
            pSettings->endGroup();

            pSettings->setValue("Datetime",sRule.date().toString("dd.MM.yyyy"));

            pSettings->beginGroup("Datelimits");
            pSettings->setValue("Startdate",sRule.dateIntervals().first.toString("dd.MM.yyyy"));
            pSettings->setValue("Enddate",sRule.dateIntervals().second.toString("dd.MM.yyyy"));
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
        if(currentRule.title() == title)
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

