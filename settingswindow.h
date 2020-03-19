#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QFileInfoList>
#include <qicon.h>
#include <qlist.h>
#include <iostream>
#include <QModelIndex>
#include <QCheckBox>
#include <qscrollbar.h>
#include <qevent.h>
#include <qcombobox.h>
#include <QDateEdit>
#include <qpair.h>
#include <QFocusEvent>
#include <qdialog.h>

#include "customcombobox.h"
#include "addruledialog.h"
#include "editruledialog.h"
#include "customdialog.h"
#include "rulescollection.h"
#include "isettingswindow.h"


using namespace std;

namespace Ui {
class SettingsWindow;
}

class SettingsWindow :
        public ISettingsWindow
{
    Q_OBJECT
public:
    explicit SettingsWindow(AbstractApplicationService *coreApplication, QWidget *parent = nullptr);
    SettingsWindow();
    ~SettingsWindow();

    // Void public functions..

    // Icons related..
    void setIconList(QList<Icon>list);

    //Rules related..
    void setRulesEnabled(bool enabled)
    {
        enableRules->setChecked(enabled);
    }

    //Non-void public functions..

    // Rules Related..
    bool rulesEnabled()
    {
        return enableRules->isChecked();
    }

public slots:

    // Update rules view
    void updateView();


protected:
    void mouseMoveEvent(QMouseEvent *event);
    void resizeNotify(QSize newSize){Q_UNUSED(newSize);}

private slots:

    // Select icon related..
    void viewClicked(QModelIndex i);

    // "Close on exit" related..
    void closeBoxClicked(bool c);

    // RulesView related..

    // Buttons related..
    void on_insertRule_2_clicked();
    void on_editRule_2_clicked();
    void on_deleteRule_2_clicked();
    void on_exitButton_clicked();
    void on_moveUpButton_2_clicked();
    void on_moveDownButton_2_clicked();

    //RuleItemView related..
    void on_ruleItemView_2_itemClicked(QTreeWidgetItem *item, int column);

    // Timer related..
    void on_countTimerActivateBox_2_toggled(bool checked);
    void on_countTImerIntervalEdit_2_returnPressed();

    void on_saveButton_clicked();

private:
    // Generel methods..

    void initializeState();

    // Icons..
    void insertIcons(QList<Icon>ic);

    // Member variables..
    Ui::SettingsWindow *ui;
    QCheckBox *closeOnBox,*countTimerEnableBox,*enableRules;
    QLineEdit *countTimerInterval;
    QListWidget *view;
    QScrollBar *vScroll;
    QPoint mOffset;
    QStringList ruleParentHeaderData,ruleChildrenHeaderData;
    QTreeWidget *rulesView;
    QList<const IRule<IDefaultRuleCondition>*>rules;
    QWidget *mView;
    AbstractApplicationService *coreApplication;
};

typedef SettingsWindow sI;

#endif // settingswindow_H
