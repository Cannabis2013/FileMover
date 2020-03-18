#ifndef RULEBUILDER_H
#define RULEBUILDER_H

#include "irulebuilder.h"
#include "defaultRuleConfiguration.h"

typedef IRuleConfiguration<IRuleCondition,
                            RulesContext::FileType,
                            RulesContext::RuleAction> IRuleConfigator;
typedef IRuleConditionConfiguration<RulesContext::RuleCriteria,
                                    RulesContext::RuleCompareCriteria,
                                    SizeLimit,SizeLimits,
                                    QDateTime,
                                    DateInterval> IRuleConditionConfigurator;
typedef IRuleBuilder<IRule<>,IRuleCondition,IRuleConfigator,IRuleConditionConfigurator> IDefaultRuleBuilder;

class RuleBuilder :
        public IDefaultRuleBuilder
{
public:
    const IRule<> *buildOrdinaryRule(const IRuleConfigator *configuration) override
    {
        IRule<> *r = new Rule;
        r->setTitle(configuration->title());
        r->setAppliesToPath(configuration->appliesTo());
        r->setDestinationPaths(configuration->destinations());
        r->setActionRuleEntity(configuration->action());
        r->setTypeFilter(configuration->type());
        r->setSubRules(configuration->conditions());
        r->setDeepScanMode(configuration->deepScanMode());

        return r;
    }

    const IRule<>* attachCriteria(const IRuleCondition *condition, IRule<>* r) override
    {
        QList<const IRuleCondition*> ruleConditions = r->conditions();
        ruleConditions.append(condition);
        r->setSubRules(ruleConditions);

        return r;
    }

    const IRuleCondition *buildSubRule(IRuleConditionConfigurator* configurator) override
    {
        IRuleCondition* sRule = new RuleCondition;
        sRule->setCriteria(configurator->criteria());
        sRule->setCompareCriteria(configurator->compareCriteria());
        sRule->setKeyWords(configurator->keywords());
        sRule->setSizeLimit(configurator->sizeLimit());
        sRule->setDate(configurator->date());
        sRule->setSizeInterval(configurator->sizeInterval());
        sRule->setDateIntervals(configurator->dates());
        sRule->setMatchWholeWords(configurator->matchWholeWords());

        return sRule;
    }
};

#endif // RULEBUILDER_H
