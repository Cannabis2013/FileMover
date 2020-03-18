#ifndef IRULEBUILDER_H
#define IRULEBUILDER_H

template<class TRule, class TCriteria, class TRuleParameters, class TCriteriaParameters>
class IRuleBuilder
{
public:
    virtual const TRule *buildOrdinaryRule(const TRuleParameters *buildParameters) = 0;
    virtual const TRule *attachCriteria(const TCriteria *criteria, TRule *r) = 0;
    virtual const TCriteria* buildSubRule(TCriteriaParameters *buildParameters) = 0;
};


#endif // IRULEBUILDER_H
