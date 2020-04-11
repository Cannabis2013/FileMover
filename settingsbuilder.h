#ifndef SETTINGSBUILDER_H
#define SETTINGSBUILDER_H

#include "isettingsbuilder.h"
#include "settingsmodel.h"

class SettingsBuilder : public ISettingsBuilder
{
    // ISettingsBuilder interface
public:
    ISettingsModel *buildSettings(const bool &closeOnExit, const bool &enableRules, const bool &enableRuleTimer, const int &ruleTimerInterval)
    {
        ISettingsModel *model = new SettingsModel;
    }
};

#endif // SETTINGSBUILDER_H
