#ifndef RULEBUILDER_H
#define RULEBUILDER_H

#include "irulebuilder.h"
#include "defaultRuleConfiguration.h"
#include "rules.h"

typedef IRuleConfiguration<IDefaultRuleCondition> IRuleConfigator;
typedef IRuleConditionConfiguration<SizeLimit,SizeLimits,
                                    QDateTime,
                                    DateInterval> IRuleConditionConfigurator;
typedef IRuleBuilder<IRule<IDefaultRuleCondition>,IDefaultRuleCondition,IRuleConfigator,IRuleConditionConfigurator> IDefaultRuleBuilder;

class RuleBuilder :
        public IDefaultRuleBuilder
{
public:
    const IRule<IDefaultRuleCondition> *buildOrdinaryRule(const IRuleConfigator *configuration) override
    {
        IRule<IDefaultRuleCondition> *r = new Rule;
        r->setTitle(configuration->title());
        r->setAppliesToPath(configuration->appliesTo());
        r->setDestinationPaths(configuration->destinations());
        r->setActionRuleEntity(configuration->action());
        r->setTypeFilter(configuration->type());
        r->setCriterias(configuration->conditions());
        r->setDeepScanMode(configuration->deepScanMode());

        return r;
    }

    const IRule<IDefaultRuleCondition>* attachCriteria(const IDefaultRuleCondition *condition, IRule<IDefaultRuleCondition>* r) override
    {
        QList<const IDefaultRuleCondition*> ruleConditions = r->conditions();
        ruleConditions.append(condition);
        r->setCriterias(ruleConditions);

        return r;
    }

    const IDefaultRuleCondition *buildSubRule(IRuleConditionConfigurator* configurator) override
    {
        IDefaultRuleCondition* condition = new RuleCondition;
        condition->setCriteria(configurator->criteria());
        condition->setCompareCriteria(configurator->compareCriteria());
        condition->setKeyWords(configurator->keywords());
        condition->setSizeLimit(configurator->sizeLimit());
        condition->setDate(configurator->date());
        condition->setSizeInterval(configurator->sizeInterval());
        condition->setDateIntervals(configurator->dates());
        condition->setMatchWholeWords(configurator->matchWholeWords());

        return condition;
    }
};

#endif // RULEBUILDER_H
