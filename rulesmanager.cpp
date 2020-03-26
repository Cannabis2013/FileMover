#include "rulesmanager.h"

rulesManager::rulesManager(const QString &appName, const QString &orgName, IDefaultRuleBuilder *ruleBuilderService):
    AbstractPersistence (appName,orgName)
{
    setRuleBuilderService(ruleBuilderService);
    readSettings();
}

rulesManager::~rulesManager()
{
    writeSettings();
}

QList<QTreeWidgetItem *> rulesManager::ruleItems() const
{
    QList<QTreeWidgetItem*>resultingList;
    for(auto r : _rules)
    {
        QStringList headerData {r->title(),ruleDefinitionsService()->fileActionEntityToString(r->actionRuleEntity()),
                    RulesContext::mergeStringList(r->destinationPaths())};
        QTreeWidgetItem *pItem = new QTreeWidgetItem(headerData);
        QIcon itemIcon = QIcon(":/My Images/Ressources/rule_icon.png");
        pItem->setIcon(0,itemIcon);
        for(auto condition : r->conditions())
        {
            QStringList hData;
            hData << ruleDefinitionsService()->buildStringFromCriteria(condition->criteria()) <<
                     ruleDefinitionsService()->buildStringFromCompareCriteria(condition->compareCriteria()) <<
                     RulesContext::ruleKeyWordToString(condition);

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

void rulesManager::addRule(const IRule<IDefaultRuleCondition> *r)
{
    _rules << r;
    emit stateChanged();
}

void rulesManager::addRules(const QList<const IRule<IDefaultRuleCondition> *> &r)
{
    _rules << r;
    emit stateChanged();
}

void rulesManager::removeRule(const QString &title)
{
    for (int i = 0; i < _rules.count(); ++i) {
        auto r = _rules.at(i);
        if(r->title() == title)
        {
            _rules.removeAt(i);
            emit stateChanged();
            return;
        }
    }
    throw QString("Item not found.");
}

const IDefaultRule *rulesManager::rule(const QString &title) const
{
    for(auto rule : _rules)
    {
        if(rule->title() == title)
            return rule;
    }

    return nullptr;
}

void rulesManager::readSettings()
{
    QList<const IRule<IDefaultRuleCondition>*>rules;
    QSettings *pSettings = persistenceSettings();
    int total = pSettings->beginReadArray("Rules");
    for (int i = 0; i < total; ++i)
    {
        pSettings->setArrayIndex(i);
        auto title = pSettings->value("Title","Title").toString();
        auto action = static_cast<RulesContext::RuleAction>(pSettings->value("Action","").toInt());
        auto appliesTo = pSettings->value("ApplyPath","Alle").toString();
        auto type = static_cast<RulesContext::FileType>(pSettings->value("Scan type filter","").toInt());
        auto destinations = RulesContext::splitString(pSettings->value("Destination paths","").toString());
        int count = pSettings->beginReadArray("Subrules");

        auto *rConfig = new RuleDefaultConfiguration;

        rConfig->setTitle(title);
        rConfig->setAction(action);
        rConfig->setAppliesTo(appliesTo);
        rConfig->setType(type);
        rConfig->setDestinations(destinations);

        QList<const IDefaultConditionConfigurator*> criteriaConfigurations;

        for (int j = 0; j < count; ++j)
        {
            pSettings->setArrayIndex(j);

            //auto mode = static_cast<RRT::CopyMode>(pSettings->value("Copymode",0).toInt());
            auto criteria = static_cast<RulesContext::RuleCriteria>(pSettings->value("Condition","").toInt());
            auto compareCriteria = static_cast<RulesContext::RuleCompareCriteria>(pSettings->value("Comparemode",0).toInt());

            auto matchWholeWords = pSettings->value("Matchwholewords",false).toBool();
            auto keyWords = RulesContext::splitString(pSettings->value("Keywords","").toString());

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
            auto date = QDateTime::fromString(pSettings->value("Datetime","").toString(),"dd.MM.yyyy");

            pSettings->beginGroup("Datelimits");

            auto lowerDate = QDateTime::fromString(pSettings->value("Startdate","01.01.2000").toString());
            auto upperDate = QDateTime::fromString(pSettings->value("Enddate","01.01.2000").toString());

            auto dates = DateInterval(lowerDate,upperDate);

            pSettings->endGroup();

            auto config = new RuleConditionDefaultConfiguration;

            config->setDate(date);
            config->setCriteria(criteria);
            config->setCompareCriteria(compareCriteria);
            config->setSizeLimit(sizeLimit);
            config->setSizeInterval(sizeLimits);
            config->setDates(dates);
            config->setMatchWholeWords(matchWholeWords);

            criteriaConfigurations << config;


        }

        auto r = ruleBuilderService()->buildRule(rConfig,criteriaConfigurations);

        pSettings->endArray();
        rules << r;
    }
    pSettings->endArray();
    addRules(rules);
}

void rulesManager::writeSettings()
{
    auto rules = this->rules();
    QSettings *pSettings = persistenceSettings();
    pSettings->remove("Rules");
    pSettings->beginWriteArray("Rules",rules.count());
    for (int i = 0; i < _rules.count(); ++i)
    {
        pSettings->setArrayIndex(i);
        auto r = _rules.at(i);
        pSettings->setValue("Title",r->title());
        pSettings->setValue("Action",r->actionRuleEntity());
        pSettings->setValue("Scan type filter",r->typeFilter());
        pSettings->setValue("ApplyPath",r->appliesToPath());
        pSettings->setValue("Destination paths",
                   RulesContext::mergeStringList(r->destinationPaths()));
        pSettings->setValue("Scan Mode",r->deepScanMode());
        QList<const IDefaultRuleCondition*>sRules = r->conditions();
        int total = sRules.count();
        pSettings->beginWriteArray("Subrules",total);
        for (int j = 0; j < total; ++j)
        {
            auto condition = sRules.at(j);
            pSettings->setArrayIndex(j);

            pSettings->setValue("Copymode",condition->copyMode());
            pSettings->setValue("Condition",condition->criteria());
            pSettings->setValue("Comparemode",condition->compareCriteria());

            pSettings->setValue("Matchwholewords",condition->matchWholeWords());
            pSettings->setValue("Keywords",RulesContext::mergeStringList(condition->keyWords()));

            pSettings->setValue("Sizelimit",condition->sizeLimit().first);
            pSettings->setValue("Sizelimitunit",condition->sizeLimit().second);

            pSettings->beginGroup("Sizelimits");
            pSettings->setValue("Minsizeinterval",condition->sizeInterval().first.first);
            pSettings->setValue("Minsizeunitinterval",condition->sizeInterval().first.second);
            pSettings->setValue("Maxsizeinterval",condition->sizeInterval().second.first);
            pSettings->setValue("Maxsizeunitinterval",condition->sizeInterval().second.second);
            pSettings->endGroup();

            pSettings->setValue("Datetime",condition->date().toString("dd.MM.yyyy"));

            pSettings->beginGroup("Datelimits");
            pSettings->setValue("Startdate",condition->dateIntervals().first.toString("dd.MM.yyyy"));
            pSettings->setValue("Enddate",condition->dateIntervals().second.toString("dd.MM.yyyy"));
            pSettings->endGroup();
        }
        pSettings->endArray();
    }
    pSettings->endArray();
}

void rulesManager::replaceRule(const IRule<IDefaultRuleCondition> *r, int index)
{
    _rules.replace(index,r);
    emit stateChanged();
}

void rulesManager::replaceRule(const IRule<IDefaultRuleCondition> *r, QString title)
{
    for (int i = 0; i < _rules.count(); ++i) {
        auto currentRule = _rules.at(i);
        if(currentRule->title() == title)
        {
            _rules.replace(i,r);
            emit stateChanged();
            return;
        }
    }
    throw QString("Item not found");
}

QList<const IDefaultRule *> rulesManager::rules() const
{
    return _rules;
}

void rulesManager::swapRule(int i, int j)
{
    if(i >= _rules.count() || j >= _rules.count())
        throw new std::out_of_range("Index out of range");
    if(i < 0 || j < 0)
        throw new std::out_of_range("Index out of range");

    _rules.swapItemsAt(i,j);

    emit stateChanged();
}

