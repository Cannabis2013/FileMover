#include "abstractRuleDialog.h"
#include "ui_abstractRuledialog.h"

abstractRuleDialog::abstractRuleDialog(QStringList folderPaths) :
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

    QStringList actionList = ruleDefs.propertyListToStrings(rD::actionProperty),
            conditionList = ruleDefs.propertyListToStrings(rD::conditionProperty),
            unitList = ruleDefs.sizeUnits();

    actionBox->addItems(actionList);
    conditionBox->addItems(conditionList);

    folderPaths << "Alle";
    applySelector->addItems(folderPaths);
    applySelector->setCurrentText("Alle");

    conditionBox->setCurrentText("Ingen betingelser");
    conditionBox->currentIndexChanged("Ingen betingelser");

    setWindowModality(Qt::ApplicationModal);
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

    rD rDefs;

    QStringList actionList = rDefs.propertyListToStrings(rD::actionProperty),
            conditionList = rDefs.propertyListToStrings(rD::conditionProperty),
            unitList = rDefs.sizeUnits();

    actionBox->addItems(actionList);
    conditionBox->addItems(conditionList);

    folderPaths << "Alle";

    applySelector->addItems(folderPaths);
    applySelector->setCurrentText("Alle");

    tempRule = r;
    actionBox->setCurrentText(rDefs.actionToString(tempRule.actionRule));
    titleSelector->setText(tempRule.title);
    applySelector->setCurrentText(tempRule.appliesToPath);
    pathSelector->setLineText(fW::mergeStringList(tempRule.destinationPath));
    deepScanRadio->setChecked(tempRule.deepScanMode);

    subRules = tempRule.subRules;

    setWindowModality(Qt::ApplicationModal);
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
    rD rDefs;
    subRule clickedSubRule = subRules.at(rowIndex);
    conditionBox->setCurrentText(rDefs.fieldConditionToString(clickedSubRule.fieldCondition));
    conditionBox->currentIndexChanged(rDefs.fieldConditionToString( clickedSubRule.fieldCondition));

    updateConditionView(clickedSubRule);
}


void abstractRuleDialog::updateView()
{
    rD rDefs;
    subRuleView->clear();
    int total = subRules.count();
    for (int i = 0; i < total; ++i)
    {
        QStringList headerData;
        subRule sRule = subRules.at(i);
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
            headerData << rC::mergeStringList(sRule.keyWords);
        }

        new QTreeWidgetItem(subRuleView,headerData);
    }
}

void abstractRuleDialog::on_actionComboBox_currentIndexChanged(const QString &arg1)
{
    rD rDefs;
    (rDefs.actionFromString(arg1) == rD::Delete) ?
                destinationFrame->hide() :
                destinationFrame->show();
}
