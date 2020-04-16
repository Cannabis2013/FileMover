#ifndef IDEFAULTRULESCOLLECTION
#define IDEFAULTRULESCOLLECTION_H

#include "iruledefinitions.h"
#include "iruleconfiguration.h"
#include "iruleinterfaces.h"
#include "irulebuilder.h"
#include "ruleservicesinjector.h"

typedef IRuleCriteria<SizeLimit,SizeLimits,QDateTime> DefaultRuleCriteria;
typedef IRule<DefaultRuleCriteria> DefaultRuleInterface;

typedef IRuleConfiguration<DefaultRuleCriteria> IDefaultRuleConfigurator;
typedef IRuleConditionConfiguration<SizeLimit,SizeLimits,QDateTime> IDefaultConditionConfigurator;
typedef IRuleBuilder<IRule<DefaultRuleCriteria>,DefaultRuleCriteria,IDefaultRuleConfigurator,IDefaultConditionConfigurator> IDefaultRuleBuilder;

#endif // IDEFAULTRULESCOLLECTION_H
