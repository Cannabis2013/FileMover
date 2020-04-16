#ifndef RULEBUILDER_H
#define RULEBUILDER_H

#include "irulebuilder.h"
#include "defaultRuleConfiguration.h"
#include "rules.h"

typedef IRuleConfiguration<DefaultRuleCriteria> IDefaultRuleConfigurator;
typedef IRuleConditionConfiguration<SizeLimit,SizeLimits,
                                    QDateTime> IDefaultConditionConfigurator;

class RuleBuilder :
        public IDefaultRuleBuilder
{
public:
    const IRule<DefaultRuleCriteria> *buildRule(const IDefaultRuleConfigurator *configuration,
                                                  const QList<const DefaultRuleCriteria*> &criterias) override
    {
        IRule<DefaultRuleCriteria> *r = new Rule;
        r->setTitle(configuration->title());
        r->setAppliesToPath(configuration->appliesTo());
        r->setDestinationPaths(configuration->destinations());
        r->setRuleAction(configuration->action());
        r->setTypeFilter(configuration->type());
        r->setCriterias(configuration->conditions());
        r->setDeepScanMode(configuration->deepScanMode());


        r->setCriterias(criterias);

        return r;
    }

    const DefaultRuleCriteria *buildCriteria(const IDefaultConditionConfigurator* configurator) override
    {
        auto condition = new RuleCondition();
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
