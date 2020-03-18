#ifndef RULESCOLLECTION_H
#define RULESCOLLECTION_H

#include "TypeDefinitions.h"
#include "iruledefinitions.h"
#include "iruleconfiguration.h"
#include "iruleinterfaces.h"
#include "irulebuilder.h"
#include "ruleservicesinjector.h"

typedef IRuleDefinitions<RulesContext::RuleType,RulesContext::RuleAction,RulesContext::RuleCriteria,RulesContext::RuleCompareCriteria,RulesContext::FileType> IDefinitions;

typedef IRuleConfiguration<IRuleCondition,RulesContext::FileType,RulesContext::RuleAction> IRuleConfigator;
typedef IRuleConditionConfiguration<RulesContext::RuleCriteria,RulesContext::RuleCompareCriteria,SizeLimit,SizeLimits,QDateTime,DateInterval> IRuleConditionConfigurator;
typedef IRuleBuilder<IRule<>,IRuleCondition,IRuleConfigator,IRuleConditionConfigurator> IDefaultRuleBuilder;


#endif // RULESCOLLECTION_H
