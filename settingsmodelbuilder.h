#ifndef SETTINGSMODELBUILDER_H
#define SETTINGSMODELBUILDER_H

#include "imodelbuilder.h"
#include "settingsmodel.h"
#include "qrect.h"
#include "defaultsettingsarguments.h"

class SettingsModelBuilder :
        public IModelBuilder<ISettingsModel<QRect>,DefaultSettingsArguments<QRect>>
{
public:
    // IModelBuilder interface
    ISettingsModel<QRect> *buildModel(const DefaultSettingsArguments<QRect> *modelArguments) override
    {
        auto model = new SettingsModel;

        model->setCloseOnExitEnabled(modelArguments->closeOnExit);
        model->setRulesEnabled(modelArguments->rulesEnabled);
        model->setRuleTimerEnabled(modelArguments->rulesTimerEnabled);
        model->setRuleTimerInterval(modelArguments->rulesTimerInterval);
        model->setGeometry(modelArguments->geometry);

        return model;
    }
};

#endif // SETTINGSMODELBUILDER_H
