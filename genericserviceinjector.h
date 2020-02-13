#ifndef GENERICSERVICEINJECTOR_H
#define GENERICSERVICEINJECTOR_H

template<class TService>
class GenericServiceInjector
{
public:
    GenericServiceInjector(TService *service)
    {
        ruleService = service;
    }

    void setService(TService *service)
    {
        ruleService = service;
    }
protected:
    TService *ruleService;
};

#endif // GENERICSERVICEINJECTOR_H
