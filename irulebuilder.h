#ifndef IRULEBUILDER_H
#define IRULEBUILDER_H

#include <qlist.h>

template<class TRule, class TRuleParameters, class TCriteriaParameters>
class IRuleBuilder
{
public:
    virtual const TRule *buildRule(const TRuleParameters *buildParameters, const QList<const TCriteriaParameters*> &attachedCriterias) = 0;
};


#endif // IRULEBUILDER_H
