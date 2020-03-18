#ifndef GENERICSERVICEINJECTOR_H
#define GENERICSERVICEINJECTOR_H

template<class TRuleDefinitions, class TRuleBuilder = int>
class RuleServicesInjector
{
public:

    virtual void setRulesDefinitionsService(TRuleDefinitions *service)
    {
        _ruleDefinitionsService = service;
    }
    TRuleDefinitions *ruleDefinitionsService() const
    {
        return _ruleDefinitionsService;
    }

    TRuleBuilder *ruleBuilderService() const
    {
        return _ruleBuilderService;
    }

    virtual void setRuleBuilderService(TRuleBuilder *service)
    {
        _ruleBuilderService = service;
    }

private:
    TRuleDefinitions *_ruleDefinitionsService;
    TRuleBuilder *_ruleBuilderService;
};

#endif // GENERICSERVICEINJECTOR_H
