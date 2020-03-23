#ifndef FILTERINGCONTEXT_H
#define FILTERINGCONTEXT_H

#include "iruleinterfaces.h"
#include "ifiltereringcontext.h"
#include "defaultfilemodelscontext.h"

class FilteringContext :
        public IFiltereringContext<IRule<IRuleCondition<SizeLimit,SizeLimits,DateInterval>>,FileModelDelegate<>>
{
public:

};

#endif // FILTERINGCONTEXT_H
