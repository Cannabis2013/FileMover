#ifndef RULEBUILDER_H
#define RULEBUILDER_H

#include "irulebuilder.h"
#include "defaultRuleConfiguration.h"
#include "rules.h"

typedef IRuleConfiguration<IDefaultRuleCondition> IDefaultRuleConfigurator;
typedef IRuleConditionConfiguration<SizeLimit,SizeLimits,
                                    QDateTime> IDefaultConditionConfigurator;

typedef IRuleBuilder<IRule<IDefaultRuleCondition>,IDefaultRuleConfigurator,IDefaultConditionConfigurator> IDefaultRuleBuilder;

class RuleBuilder :
        public IDefaultRuleBuilder
{
public:
    const IRule<IDefaultRuleCondition> *buildRule(const IDefaultRuleConfigurator *configuration,
                                                  const QList<const IDefaultConditionConfigurator*> &conditionConfigurations) override
    {
        IRule<IDefaultRuleCondition> *r = new Rule;
        r->setTitle(configuration->title());
        r->setAppliesToPath(configuration->appliesTo());
        r->setDestinationPaths(configuration->destinations());
        r->setActionRuleEntity(configuration->action());
        r->setTypeFilter(configuration->type());
        r->setCriterias(configuration->conditions());
        r->setDeepScanMode(configuration->deepScanMode());

        QList<const IDefaultRuleCondition*> criterias;

        for (auto config : conditionConfigurations) {
            auto criteria = buildCriteria(config);
            criterias << criteria;
        }

        r->setCriterias(criterias);

        return r;
    }

private:
    const IDefaultRuleCondition *buildCriteria(const IDefaultConditionConfigurator* configurator)
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
