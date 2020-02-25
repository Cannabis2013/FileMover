#ifndef abstractRuleDialog_H
#define abstractRuleDialog_H

#include <QWidget>
#include <QStringList>
#include <QComboBox>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QLineEdit>
#include <QDate>
#include <qdatetime.h>
#include <qspinbox.h>
#include <QStackedWidget>
#include <QCheckBox>
#include <qradiobutton.h>
#include <qpushbutton.h>
#include <QFocusEvent>
#include <qdialog.h>

#include "abstracttextselectordialog.h"
#include "rulepathselector.h"
#include "conditionwidget.h"
#include "abstractcoreapplication.h"
#include "abstractframe.h"

namespace Ui {
class AbstractRuleDialog;
}

class AbstractRuleDialog : public AbstractFrame
{
    Q_OBJECT
public:
    explicit AbstractRuleDialog(QStringList watchFolders, IDefinitions *rService);
    ~AbstractRuleDialog();

signals:
    void sendRule(Rule R);
    void replaceRule(Rule rule, QString title);

protected slots:

    // Actionbox & conditionbox related..
    void on_actionComboBox_currentIndexChanged(const QString &arg1);
    void on_conditionComboBox_currentIndexChanged(const QString &arg1);

    /*
     * Pure virtual methods
     */

    // Add rule button..
    virtual void on_addButton_clicked()=0;
    // Add subrule..
    virtual void on_addSubRule_clicked()=0;
    // Remove subrule..
    virtual void on_removeSubRule_clicked()=0;

    // Subruleview related..
    void on_treeWidget_doubleClicked(const QModelIndex &index);

protected:

    // Event handling

    void closeEvent(QCloseEvent *event);

    // Protected methods
    enum buttonType {acceptButton,cancelButton};
    void setButtonText(QString txt,buttonType bType = buttonType::acceptButton);
    void resizeNotify(QSize newSize);

    // Protected member variables
    conditionWidget *condWidget;
    rulePathSelector *pathSelector;
    QComboBox *actionBox,
        *conditionBox,
        *fileTypeSelector,
        *applySelector;
    QDateEdit *dateTime;
    QFrame *destinationFrame;
    QLineEdit *titleSelector,
        *keyWordSelector,
        *destinationPathSelector;
    QList<SubRule> subRules;
    QPushButton *addBut;
    QSpinBox *minSizeSelector,
        *maxSizeSelector,
        *fixedSizeSelector;
    QStackedWidget *keywordViewer;
    QTreeWidget *subRuleView;
    IDefinitions *ruleService;

    void updateView();
    void resetSubForm();
    void resetAllForm();

    void updateConditionView(SubRule &sR);

private:
    Ui::AbstractRuleDialog *ui;
};

#endif // abstractRuleDialog_H
