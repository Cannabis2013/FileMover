#ifndef SETTINGSWINDOWBUILDER_H
#define SETTINGSWINDOWBUILDER_H

#include "isettingswindow.h"
#include "ISettingsWindowBuilder.h"
#include "abstractapplicationservice.h"
#include "settingswindow.h"

class SettingsWindowBuilder : public ISettingsWindowBuilder<ISettingsWindow,AbstractApplicationService>
{
public:


    // ISettingsWindowBuilder interface
public:
    ISettingsWindow *buildSettingsWindow(AbstractApplicationService *parameter)
    {
        return new SettingsWindow(parameter);
    }
};

#endif // SETTINGSWINDOWBUILDER_H
