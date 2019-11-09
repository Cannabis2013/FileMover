#ifndef SETTINGSDELEGATE_H
#define SETTINGSDELEGATE_H

#include <qrect.h>

struct SettingsDelegate
{
    bool closeOnExit;
    bool ruleTimerEnabled;
    bool rulesEnabled;
    int ruleCountInterval;

    QRect mainGuiGeometry;
};

#endif // SETTINGSDELEGATE_H
