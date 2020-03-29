#ifndef IRULEBUILDER_H
#define IRULEBUILDER_H

#include <qlist.h>

template<class TRule,class TCriteria, class TRuleParameters, class TCriteriaParameters>
class IRuleBuilder
{
public:
    virtual const TRule *buildRule(const TRuleParameters *buildParameters, const QList<const TCriteria*> &criterias) = 0;
    virtual const TCriteria *buildCriteria(const TCriteriaParameters*buildParameters) = 0;
};


#endif // IRULEBUILDER_H
