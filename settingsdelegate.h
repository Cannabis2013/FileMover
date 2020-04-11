#ifndef SETTINGSDELEGATE_H
#define SETTINGSDELEGATE_H

#include <iostream>
#include "isettingsmodel.h"
#include <inheritexceptiondelegate.h>

using namespace std;

class SettingsModel : public ISettingsModel
{
public:
    bool isCloseOnExitEnabled() const
    {
        return _closeOnExit;
    }
    bool isRuleTimerEnabled() const
    {
        return _ruleTimerEnabled;
    }
    bool isRulesEnabled() const
    {
        return _rulesEnabled;
    }
    int ruleTimerInterval() const
    {
        return _ruleCountInterval;
    }
    QRect geometry() const
    {
        return _mainGuiGeometry;
    }

private:
    bool _closeOnExit;
    bool _ruleTimerEnabled;
    bool _rulesEnabled;
    int _ruleCountInterval;

    QRect _mainGuiGeometry;
};

#endif // SETTINGSDELEGATE_H
