#ifndef IDEFAULTRULESCOLLECTION
#define IDEFAULTRULESCOLLECTION_H

#include "iruledefinitions.h"
#include "iruleconfiguration.h"
#include "iruleinterfaces.h"
#include "irulebuilder.h"
#include "ruleservicesinjector.h"

typedef IRuleCondition<SizeLimit,SizeLimits,QDateTime> IDefaultRuleCondition;
typedef IRule<IDefaultRuleCondition> IDefaultRule;

typedef IRuleConfiguration<IDefaultRuleCondition> IDefaultRuleConfigurator;
typedef IRuleConditionConfiguration<SizeLimit,SizeLimits,QDateTime> IDefaultConditionConfigurator;
typedef IRuleBuilder<IRule<IDefaultRuleCondition>,IDefaultRuleConfigurator,IDefaultConditionConfigurator> IDefaultRuleBuilder;

#endif // IDEFAULTRULESCOLLECTION_H
