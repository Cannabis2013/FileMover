#ifndef ISETTINGSMODELBUILDER_H
#define ISETTINGSMODELBUILDER_H

#include "isettingsmodel.h"
template<class TGeometry>
class ISettingsBuilder
{
public:
    virtual ISettingsModel<TGeometry> *buildSettings(const bool &closeOnExit,
                                          const bool &enableRules,
                                          const bool &enableRuleTimer,
                                          const int &ruleTimerInterval,
                                          const TGeometry &geometry) = 0;
};

#endif // ISETTINGSMODELBUILDER_H
