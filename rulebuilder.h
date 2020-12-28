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
    const DefaultRuleInterface *buildRule(const IDefaultRuleConfigurator *configuration,
                                                  const QList<const DefaultRuleCriteria*> &criterias) override
    {
        DefaultRuleInterface *r = new Rule;
        r->setTitle(configuration->title());
        r->setAppliesToPath(configuration->appliesTo());
        r->setDestinationPaths(configuration->destinations());
        r->setRuleAction(configuration->action());
        r->setTypeFilter(configuration->type());
        r->setCriterias(configuration->conditions());
        r->setDeepScanMode(configuration->deepScanMode());
        r->setType(RulesContext::Rule);
        r->setId(QUuid::createUuid());

        r->setCriterias(criterias);

        return r;
    }

    const DefaultRuleCriteria *buildCriteria(const IDefaultConditionConfigurator* configurator) override
    {
        auto criteria = new RuleCondition();

        criteria->setCriteria(configurator->criteria());
        criteria->setCompareCriteria(configurator->compareCriteria());
        criteria->setKeyWords(configurator->keywords());
        criteria->setSizeLimit(configurator->sizeLimit());
        criteria->setDate(configurator->date());
        criteria->setSizeInterval(configurator->sizeInterval());
        criteria->setDateIntervals(configurator->dates());
        criteria->setMatchWholeWords(configurator->matchWholeWords());
        criteria->setType(RulesContext::Criteria);
        criteria->setId(QUuid::createUuid());

        return criteria;
    }
};

#endif // RULEBUILDER_H
