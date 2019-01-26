#ifndef settingsWindow_H
#define settingsWindow_H

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

#include "mycombobox.h"
#include "addruledialog.h"
#include "editruledialog.h"
#include "settingscontroller.h"
#include "widgetform.h"

using namespace std;

namespace Ui {
class settingsWindow;
}

class settingsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit settingsWindow(QWidget *parent,sC*sCon,ruleController *rCon);
    ~settingsWindow();

    enum ruleMode{ruleOut,parallelMode};

    // Void public functions..

    // Icons related..
    void setIconList(QList<myIcon>list);

    //Rules related..
    void setRulesEnabled(bool r);

    //Non-void public functions..

    // Rules Related..
    bool rulesEnabled();

signals:
    void countTimerActivated(bool tf);
    void sendInterval(int minutes);
    void sendCheckBox(bool a);
    void iconSelected(QIcon i);
    void enableTimer(bool et);
    void widgetHasQuitted();

protected:
    void focusOutEvent(QFocusEvent *event);
    void changeEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void closeEvent(QCloseEvent *event);
private slots:

    // Select icon related..
    void viewClicked(QModelIndex i);

    // "Close on exit" related..
    void closeBoxClicked(bool c);

    // RulesView related..

    // Rules related..
    void recieveRule(rule r);
    void recieveModifiedRule(rule r,int index);

    // Buttons related..
    void on_insertRule_2_clicked();
    void on_lukKnap_2_clicked();
    void on_fortrydKnap_2_clicked();
    void on_deleteRule_2_clicked();
    void on_exitButton_clicked();
    void on_moveUpButton_2_clicked();
    void on_moveDownButton_2_clicked();
    void on_editRule_2_clicked();

    //RuleItemView related..
    void on_ruleItemView_2_itemClicked(QTreeWidgetItem *item, int column);

    // Timer related..
    void on_countTimerActivateBox_2_toggled(bool checked);
    void on_countTImerIntervalEdit_2_returnPressed();


private:
    // Generel methods..

    // Icons..
    void insertIcons(QList<myIcon>ic);

    // Rules..
    void updateRulesView();

    // Member variables..
    Ui::settingsWindow *ui;
    rC *rControl;
    sC *sControl;
    QCheckBox *closeOnBox,*countTimerEnableBox,*enableRules;
    QLineEdit *countTimerInterval;
    QListWidget *view;
    QScrollBar *vScroll;
    QPoint mOffset;
    QStringList ruleParentHeaderData,ruleChildrenHeaderData;
    QTreeWidget *rulesView;
    QList<rule>rules;
    QWidget *mView;
};

typedef settingsWindow sI;

#endif // settingsWindow_H
