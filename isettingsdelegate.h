#ifndef ISETTINGSDELEGATE_H
#define ISETTINGSDELEGATE_H

#include <qrect.h>
#include "imodeldelegates.h"


class ISettingsDelegate : public IModelDelegate<Model>
{
public:
    virtual bool closeOnExit() const = 0;
    virtual bool ruleTimerEnabled() const = 0;
    virtual bool rulesEnabled() const = 0;
    virtual int ruleCountInterval() const = 0;
    virtual QRect mainGuiGeometry() const = 0;
};

#endif // ISETTINGSDELEGATE_H
