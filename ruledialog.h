#ifndef ruleDialog_H
#define ruleDialog_H

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

#include "textpathedit.h"
#include "rulepathselector.h"
#include "conditionWidget.h"


namespace Ui {
class abstractRuleDialog;
}

class abstractRuleDialog : public QWidget
{
    Q_OBJECT
public:
    explicit abstractRuleDialog(QStringList folderPaths,QWidget *parent = nullptr);
    abstractRuleDialog(rule r, QStringList folderPaths);
    ~abstractRuleDialog();

signals:
    void sendRule(rule R);
    void sendModifiedRule(rule r, int index);

protected slots:

    // Actionbox & conditionbox related..
    void on_actionComboBox_currentIndexChanged(const QString &arg1);
    void on_conditionComboBox_currentIndexChanged(const QString &arg1);

    // Add rule..
    virtual void on_addButton_clicked(){}
    // Add subrule..
    virtual void on_addSubRule_clicked(){}
    // Remove subrule..
    virtual void on_removeSubRule_clicked(){}

    // Subruleview related..
    void on_treeWidget_doubleClicked(const QModelIndex &index);

protected:
    // Protected methods
    enum buttonType {acceptButton,cancelButton};
    void setButtonText(QString txt,buttonType bType = buttonType::acceptButton);

    // Protected member variables
    conditionWidget *condWidget;
    rule tempRule;
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
    QList<subRule> subRules;
    QPushButton *addBut;
    QRadioButton *deepScanRadio;
    QSpinBox *minSizeSelector,
        *maxSizeSelector,
        *fixedSizeSelector;
    QStackedWidget *keywordViewer;
    QTreeWidget *subRuleView;

    void updateView();
    void resetSubForm();
    void resetAllForm();

    void updateConditionView(subRule &sR);

private:
    Ui::abstractRuleDialog *ui;

    friend class settingsWindow;
};

#endif // ruleDialog_H
