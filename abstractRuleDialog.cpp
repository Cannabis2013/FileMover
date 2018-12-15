#include "abstractRuleDialog.h"
#include "ui_abstractRuledialog.h"

abstractRuleDialog::abstractRuleDialog(QStringList folderPaths, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::abstractRuleDialog)
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
    rD ruleDefs;

    QStringList actionList = ruleDefs.actionList,
            conditionList = ruleDefs.conditionalList,
            unitList = ruleDefs.sizeUnits();

    actionBox->addItems(actionList);
    conditionBox->addItems(conditionList);

    folderPaths << "Alle";
    applySelector->addItems(folderPaths);
    applySelector->setCurrentText("Alle");

    conditionBox->setCurrentText("Ingen betingelser");
    conditionBox->currentIndexChanged("Ingen betingelser");

}

abstractRuleDialog::abstractRuleDialog(rule r,QStringList folderPaths):
    ui(new Ui::abstractRuleDialog)
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

    rD ruleDefs;

    QStringList actionList = ruleDefs.actionList,
            conditionList = ruleDefs.conditionalList,
            unitList = ruleDefs.sizeUnits();

    actionBox->addItems(actionList);
    conditionBox->addItems(conditionList);

    folderPaths << "Alle";

    applySelector->addItems(folderPaths);
    applySelector->setCurrentText("Alle");

    tempRule = r;

    actionBox->setCurrentText(tempRule.action);
    titleSelector->setText(tempRule.title);
    applySelector->setCurrentText(tempRule.appliesToPath);
    pathSelector->setLineText(fW::mergeStringList(tempRule.destinationPath));
    deepScanRadio->setChecked(tempRule.deepScanMode);

    subRules = tempRule.subRules;
}

abstractRuleDialog::~abstractRuleDialog()
{
    delete ui;
    delete this;
}

void abstractRuleDialog::resetSubForm()
{
    actionBox->setCurrentIndex(0);
    conditionBox->setCurrentIndex(0);
    dateTime->clear();
    keyWordSelector->clear();
    pathSelector->clear();
}

void abstractRuleDialog::resetAllForm()
{
    titleSelector->clear();
    actionBox->setCurrentIndex(0);
    conditionBox->setCurrentIndex(0);
    dateTime->clear();
    keyWordSelector->clear();
    pathSelector->clear();

    subRuleView->clear();
}

void abstractRuleDialog::updateConditionView(subRule &sR)
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

void abstractRuleDialog::on_conditionComboBox_currentIndexChanged(const QString &arg1)
{
    condWidget->setMode(arg1);
}

void abstractRuleDialog::on_treeWidget_doubleClicked(const QModelIndex &index)
{
    int rowIndex = index.row();

    subRule clickedSubRule = subRules.at(rowIndex);
    conditionBox->setCurrentText(rD::fieldConditionalToString(clickedSubRule.fieldCondition));
    conditionBox->currentIndexChanged(rD::fieldConditionalToString(clickedSubRule.fieldCondition));

    updateConditionView(clickedSubRule);
}

void abstractRuleDialog::updateView()
{
    subRuleView->clear();
    int total = subRules.count();
    for (int i = 0; i < total; ++i)
    {
        QStringList headerData;
        subRule sRule = subRules.at(i);
        rD::fileFieldCondition condition = sRule.fieldCondition;

        headerData << rD::fieldConditionalToString(condition);
        headerData << rD::compareModeToString(sRule.fileCompareMode);

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
            headerData << rC::mergeStringList(sRule.keyWords);
        }

        new QTreeWidgetItem(subRuleView,headerData);
    }
}

void abstractRuleDialog::on_actionComboBox_currentIndexChanged(const QString &arg1)
{
    (rD::actionFromString(arg1) == rD::Delete) ?
                destinationFrame->hide() :
                destinationFrame->show();
}
