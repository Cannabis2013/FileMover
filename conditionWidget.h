#ifndef CONDITIONWIDGET_H
#define CONDITIONWIDGET_H

#include <QWidget>
#include <qpair.h>
#include <qstring.h>
#include <qspinbox.h>
#include <qcombobox.h>
#include <QStackedWidget>
#include <qcheckbox.h>
#include <QTextCodec>
#include <QDateEdit>

#include "rulecontroller.h"


namespace Ui {
class conditionWidget;
}

class conditionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit conditionWidget(QWidget *parent = nullptr);
    ~conditionWidget();

private slots:

    void on_modeSelector_currentIndexChanged(int index);
    void on_folderSelector_toggled(bool checked);
    void on_fileSelector_toggled(bool checked);

private:
    // void members..

    // Editrule related..

    void setKeyWords(QString kW);
    void setConditionalFixedSize(QPair<int,QString>fSize, rD::compareMode cMode);
    void setConditionalIntervalSize(QPair<QPair<int,QString>,QPair<int,QString>> iSize);
    void setFixedDate(QPair<rD::compareMode,myDateTime> dt);
    void setIntervalDate(QPair<myDateTime,myDateTime> iDate);
    void setTypeValues(bW::iteratorMode tMode);

    // General view related..
    void setMode(QString condition);
    void setCurrentView(QString txt);
    void setCurrentView(int index);
    void setCompareView(rD::compareMode compare);

    // General values and other..
    void initDefaultOperators();
    void initDefaultUnits();

    void resetValues();

    // non-void members..
    //  Comparemode
    rD::compareMode currentCompareMode();
    QString currentMode() const {return mainModeSelector->currentText();}


    // Current view index..
    int currentViewIndex() const {return mainModeView->currentIndex();}
    // Filename/filepath related..
    QString keyWordValues() const;
    // Get size values..
    QPair<int,QString> fixedSizeValues() const;
    QPair<QPair<int,QString>,QPair<int,QString>> intervalSizeValues() const;
    // Get Date values..
    QPair<rD::compareMode,myDateTime>fixedConditionalDate() const;
    QPair<myDateTime,myDateTime>intervalDates() const;
    // Get type values..
    bW::iteratorMode typeMode() const;

    // Member variables...
    QCheckBox *folderCheckBox,*fileCheckBox;
    QComboBox *mainModeSelector,
    *fixedSizeUnitSelector,
    *fixedSizeModeSelector,
    *sizeMinIntervalUnitSelector,
    *sizeMaxIntervalUnitSelector,
    *dateOperatorSelector;
    QLineEdit *keyWordSelector;
    QPair<int,int>indexRange;
    QSpinBox *fixedSizeSelector,*minIntervalSizeSelector,*maxIntervalSizeSelector;
    QStackedWidget *mainModeView;
    rD::fileFieldCondition currentFileModeRule;
    Ui::conditionWidget *ui;

    friend class ruleDialog;
    friend class editRuleDialog;
    friend class addRuleDialog;
};

#endif // CONDITIONWIDGET_H
