#ifndef SETTINGSDELEGATE_H
#define SETTINGSDELEGATE_H

#include <iostream>
#include "isettingsdelegate.h"
#include <inheritexceptiondelegate.h>

using namespace std;

struct SettingsModel : public Model
{
    bool closeOnExit;
    bool ruleTimerEnabled;
    bool rulesEnabled;
    int ruleCountInterval;

    QRect mainGuiGeometry;
};

/*
 * Template argument must subclass Model; otherwise a custom template exception is thrown
 */

class SettingsDelegate : public ISettingsDelegate
{
public:

public:
    // IModelDelegate interface
    QUuid modelId()
    {
        return _model->id;
    }
    const SettingsModel *model() const
    {
        return _model;
    }
    SettingsModel *modelValue() const
    {
        return new SettingsModel(*_model);
    }
    DefaultModelType type()
    {
        return non_tree;
    }

    // ISettingsDelegate interface
    bool closeOnExit() const
    {
        return _model->closeOnExit;
    }
    bool ruleTimerEnabled() const
    {
        return _model->ruleTimerEnabled;
    }
    bool rulesEnabled() const
    {
        return _model->rulesEnabled;
    }
    int ruleCountInterval() const
    {
        return _model->ruleCountInterval;
    }
    QRect mainGuiGeometry() const
    {
        return _model->mainGuiGeometry;
    }

private:
    SettingsDelegate(SettingsModel model)
    {
        if(!is_base_of_v<Model,Q_TYPEOF(model)>)
            throw new InheritExceptionDelegate<Model,Q_TYPEOF(model)>();

        _model = new SettingsModel(model);
    }

    const SettingsModel *_model;

    // TODO: Friend a builder
    friend class SettingsDelegateBuilder;
};

class SettingsDelegateBuilder
{
public:
    static const SettingsDelegate* buildSettingsDelegate(bool enableCloseOnExit, bool enableTimer, bool enableRules, int ruleCountInterval,QRect guiGeometry)
    {
        SettingsModel model;
        model.closeOnExit = enableCloseOnExit;
        model.ruleTimerEnabled = enableTimer;
        model.rulesEnabled = enableRules;
        model.ruleCountInterval = ruleCountInterval;
        model.mainGuiGeometry = guiGeometry;
        model.id = QUuid::createUuid();

        return new const SettingsDelegate(model);
    }

    static const SettingsDelegate *buildDefaultDelegate()
    {
        return new const SettingsDelegate(SettingsModel());
    }
};

#endif // SETTINGSDELEGATE_H
