#ifndef SETTINGSBUILDER_H
#define SETTINGSBUILDER_H

#include "isettingsbuilder.h"
#include "settingsmodel.h"

class SettingsBuilder : public ISettingsBuilder<QRect>
{
    // ISettingsBuilder interface
public:
    ISettingsModel<QRect> * buildSettings(const bool &closeOnExit,
                                          const bool &enableRules,
                                          const bool &enableRuleTimer,
                                          const int &ruleTimerInterval,
                                          const QRect &geometry) override
    {

    }
};

#endif // SETTINGSBUILDER_H
