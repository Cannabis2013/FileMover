#ifndef ISETTINGSMODELBUILDER_H
#define ISETTINGSMODELBUILDER_H

#include "isettingsmodel.h"

class ISettingsBuilder
{
public:
    virtual ISettingsModel *buildSettings(const bool &closeOnExit,
                                          const bool &enableRules,
                                          const bool &enableRuleTimer,
                                          const int &ruleTimerInterval) = 0;
};

#endif // ISETTINGSMODELBUILDER_H
