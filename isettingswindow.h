#ifndef ISETTINGSWINDOW_H
#define ISETTINGSWINDOW_H

#include "icon.h"
#include "abstractapplicationservice.h"
#include "defaultinjector.h"
#include "abstractframe.h"

class ISettingsWindow :
        public AbstractFrame,
        public DefaultServiceInjector
{
    Q_OBJECT
public:
    ISettingsWindow(QWidget *frame):
        AbstractFrame(frame)
    {

    }

    virtual ~ISettingsWindow() = default;

    // Void public functions..

    // Icons related..
    virtual void setIconList(QList<Icon>list) = 0;

    //Rules related..
    virtual void setRulesEnabled(bool r) = 0;

    //Non-void public functions..

    // Rules Related..
    virtual bool rulesEnabled() = 0;

public slots:

    // Update rules view
    virtual void updateView() = 0;

signals:
    void countTimerActivated(bool tf);
    void sendInterval(int minutes);
    void sendCheckBox(bool a);
    void iconSelected(QIcon i);
    void enableTimer(bool et);
    void widgetHasQuitted();
};

#endif // ISETTINGSWINDOW_H
