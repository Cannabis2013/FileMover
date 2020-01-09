#ifndef SETTINGSDELEGATE_H
#define SETTINGSDELEGATE_H

#include <qrect.h>

struct Settings_Structure
{
    bool closeOnExit;
    bool ruleTimerEnabled;
    bool rulesEnabled;
    int ruleCountInterval;

    QRect mainGuiGeometry;
};

#endif // SETTINGSDELEGATE_H
