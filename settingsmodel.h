#ifndef SETTINGSDELEGATE_H
#define SETTINGSDELEGATE_H

#include <qrect.h>
#include "isettingsmodel.h"

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
        return _ruleTimerInterval;
    }
    QRect geometry() const
    {
        return _geometry;
    }

private:
    bool _closeOnExit, _ruleTimerEnabled, _rulesEnabled;
    int _ruleTimerInterval;
    QRect _geometry;
};


#endif // SETTINGSDELEGATE_H
