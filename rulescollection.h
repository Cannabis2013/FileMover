#ifndef RULESCOLLECTION_H
#define RULESCOLLECTION_H

#include "TypeDefinitions.h"
#include "iruledefinitions.h"
#include "iruleconfiguration.h"
#include "iruleinterfaces.h"
#include "irulebuilder.h"
#include "ruleservicesinjector.h"

typedef IRuleConfiguration<IDefaultRuleCondition> IRuleConfigator;
typedef IRuleConditionConfiguration<SizeLimit,SizeLimits,QDateTime,DateInterval> IRuleConditionConfigurator;
typedef IRuleBuilder<IRule<IDefaultRuleCondition>,IDefaultRuleCondition,IRuleConfigator,IRuleConditionConfigurator> IDefaultRuleBuilder;

#endif // RULESCOLLECTION_H
