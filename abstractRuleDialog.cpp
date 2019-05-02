#include "AbstractRuleDialog.h"
#include "ui_abstractRuledialog.h"

AbstractRuleDialog::AbstractRuleDialog(QStringList watchFolders) :
    ui(new Ui::AbstractRuleDialog)
{
    ui->setupUi(this);

    addBut = ui->addButton;
    actionBox = ui->actionComboBox;
    applySelector = ui->appliesSelector;
    conditionBox = ui->conditionComboBox;
    destinationFrame= ui->DestinationFrame;
    pathSelector = ui->textPathEdit;
    subRuleView = ui->treeWidget;
    titleSelector = ui->titleBox;
    deepScanRadio = ui->deepScanRadioButton;
    condWidget = ui->conditionView;

    QStringList actionList = ruleDefs.propertyListToStrings(rD::actionProperty),
            conditionList = ruleDefs.propertyListToStrings(rD::conditionProperty),
            unitList = ruleDefs.sizeUnits();

    actionBox->addItems(actionList);
    conditionBox->addItems(conditionList);

    applySelector->addItem( "Alle");
    applySelector->addItems(watchFolders);
    applySelector->setCurrentText("Alle");

    conditionBox->setCurrentText("Ingen betingelser");
    conditionBox->currentIndexChanged("Ingen betingelser");

    qRegisterMetaType<Rule>("Rule");
}

AbstractRuleDialog::~AbstractRuleDialog()
{
    delete ui;
    delete this;
}

void AbstractRuleDialog::resetSubForm()
{
    actionBox->setCurrentIndex(0);
    conditionBox->setCurrentIndex(0);
    dateTime->clear();
    keyWordSelector->clear();
    pathSelector->clear();
}

void AbstractRuleDialog::resetAllForm()
{
    titleSelector->clear();
    actionBox->setCurrentIndex(0);
    conditionBox->setCurrentIndex(0);
    dateTime->clear();
    keyWordSelector->clear();
    pathSelector->clear();

    subRuleView->clear();
}

void AbstractRuleDialog::updateConditionView(SubRule &sR)
{
    rD::fileFieldCondition cond = sR.fieldCondition;
    rD::compareMode comp = sR.fileCompareMode;
    if(cond == rD::filepathMode || cond == rD::extensionMode)
    {
        condWidget->setKeyWords(sR.Keyword());
        condWidget->setCompareView(sR.fileCompareMode);
    }
    else if(cond == rD::sizeMode && comp != rD::interval)
    {
        condWidget->setConditionalFixedSize(sR.sizeLimit,sR.fileCompareMode);
    }
    else if(cond == rD::sizeMode && comp== rD::interval)
    {
        condWidget->setConditionalIntervalSize(sR.sizeIntervalLimits);
    }
    else if((cond == rD::dateCreatedMode || cond == rD::dateModifiedMode) &&
            comp != rD::interval)
    {
        condWidget->setFixedDate(sR.fixedDate);
    }
    else if((cond == rD::dateCreatedMode || cond == rD::dateModifiedMode) &&
            comp == rD::interval)
    {
        condWidget->setIntervalDate(sR.intervalDate);
    }
    else if(cond == rD::typeMode)
    {
        condWidget->setTypeValues(sR.typeMode);
    }
}

void AbstractRuleDialog::on_conditionComboBox_currentIndexChanged(const QString &arg1)
{
    condWidget->setMode(arg1);
}

void AbstractRuleDialog::on_treeWidget_doubleClicked(const QModelIndex &index)
{
    int rowIndex = index.row();
    rD rDefs;
    SubRule clickedSubRule = subRules.at(rowIndex);
    conditionBox->setCurrentText(rDefs.fieldConditionToString(clickedSubRule.fieldCondition));
    conditionBox->currentIndexChanged(rDefs.fieldConditionToString( clickedSubRule.fieldCondition));

    updateConditionView(clickedSubRule);
}

void AbstractRuleDialog::closeEvent(QCloseEvent *event)
{
    emit destroyed();
}


void AbstractRuleDialog::updateView()
{
    rD rDefs;
    subRuleView->clear();
    int total = subRules.count();
    for (int i = 0; i < total; ++i)
    {
        QStringList headerData;
        SubRule sRule = subRules.at(i);
        rD::fileFieldCondition condition = sRule.fieldCondition;

        headerData << rDefs.fieldConditionToString(condition);
        headerData << rDefs.compareToString(sRule.fileCompareMode);

        if((condition == rD::dateCreatedMode || condition == rD::dateModifiedMode) &&
                sRule.fileCompareMode != rD::interval)
        {
            headerData << sRule.Keyword();
        }
        else if((condition == rD::dateCreatedMode || condition == rD::dateModifiedMode) &&
                sRule.fileCompareMode == rD::interval)
        {
            headerData << sRule.dateLimitsToString();
        }
        else if(condition == rD::sizeMode &&
                sRule.fileCompareMode != rD::interval)
        {
           headerData << sRule.Keyword();
        }
        else if(condition == rD::sizeMode &&
                sRule.fileCompareMode == rD::interval)
        {
            headerData << sRule.sizeLimitsToString();
        }
        else if(condition == rD::typeMode)
        {
            headerData << sRule.Keyword();
        }
        else
        {
            headerData << rulesManager::mergeStringList(sRule.keyWords);
        }

        new QTreeWidgetItem(subRuleView,headerData);
    }
}

void AbstractRuleDialog::on_actionComboBox_currentIndexChanged(const QString &arg1)
{
    rD rDefs;
    (rDefs.actionFromString(arg1) == rD::Delete) ?
                destinationFrame->hide() :
                destinationFrame->show();
}
