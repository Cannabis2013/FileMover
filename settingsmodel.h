#ifndef SETTINGSDELEGATE_H
#define SETTINGSDELEGATE_H

#include <qrect.h>
#include "isettingsmodel.h"

class SettingsModel : public ISettingsModel<QRect>
{
public:
    bool isCloseOnExitEnabled() const  override
    {
        return _closeOnExit;
    }
    bool isRuleTimerEnabled() const  override
    {
        return _ruleTimerEnabled;
    }
    bool isRulesEnabled() const  override
    {
        return _rulesEnabled;
    }
    int ruleTimerInterval() const  override
    {
        return _ruleCountInterval;
    }
    QRect geometry() const override
    {
        return _mainGuiGeometry;
    }

    void setCloseOnExitEnabled(const bool &enable) override
    {
        _closeOnExit = enable;
    }
    void setRuleTimerEnabled(const bool &enable) override
    {
        _ruleTimerEnabled = enable;
    }
    void setRulesEnabled(const bool &enable) override
    {
        _rulesEnabled = enable;
    }
    void setRuleTimerInterval(const int &interval) override
    {
        _ruleCountInterval = interval;
    }
    void setGeometry(const QRect &geometry) override
    {
        _mainGuiGeometry = geometry;
    }

private:
    bool _closeOnExit;
    bool _ruleTimerEnabled;
    bool _rulesEnabled;
    int _ruleCountInterval;

    QRect _mainGuiGeometry;
};


#endif // SETTINGSDELEGATE_H
