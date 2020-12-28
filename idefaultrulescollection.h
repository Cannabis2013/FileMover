#ifndef IDEFAULTRULESCOLLECTION
#define IDEFAULTRULESCOLLECTION_H

#include "iruledefinitions.h"
#include "iruleconfiguration.h"
#include "iruleinterfaces.h"
#include "irulebuilder.h"
#include "ruleservicesinjector.h"
#include "quuid.h"

typedef IRuleCriteria<SizeLimit,SizeLimits,QDateTime,int, QUuid> DefaultRuleCriteria;
typedef IRule<DefaultRuleCriteria,int,QUuid> DefaultRuleInterface;

typedef IRuleConfiguration<DefaultRuleCriteria> IDefaultRuleConfigurator;
typedef IRuleConditionConfiguration<SizeLimit,SizeLimits,QDateTime> IDefaultConditionConfigurator;
typedef IRuleBuilder<DefaultRuleInterface,DefaultRuleCriteria,IDefaultRuleConfigurator,IDefaultConditionConfigurator> IDefaultRuleBuilder;

#endif // IDEFAULTRULESCOLLECTION_H
