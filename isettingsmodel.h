#ifndef ISETTINGSDELEGATE_H
#define ISETTINGSDELEGATE_H

#include <qrect.h>

class ISettingsModel
{
public:
    virtual bool isCloseOnExitEnabled() const = 0;
    virtual bool isRuleTimerEnabled() const = 0;
    virtual bool isRulesEnabled() const = 0;
    virtual int ruleTimerInterval() const = 0;
    virtual QRect geometry() const = 0;
};

#endif // ISETTINGSDELEGATE_H
