#include "AbstractRuleDialog.h"
#include "ui_abstractRuledialog.h"

AbstractRuleDialog::AbstractRuleDialog(QStringList watchFolders) :
    AbstractFrameImplementable(),
    ui(new Ui::AbstractRuleDialog)
{
    ui->setupUi(this);

    addBut = ui->addButton;
    actionBox = ui->actionComboBox;
    applySelector = ui->appliesSelector;
    conditionBox = ui->conditionComboBox;
    destinationFrame= ui->DestinationFrame;
    fileTypeSelector = ui->typeFilterSelector;
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

    applySelector->addItem( "Alle");
    applySelector->addItems(watchFolders);
    applySelector->setCurrentText("Alle");

    conditionBox->setCurrentText("Ingen betingelser");
    conditionBox->currentIndexChanged("Ingen betingelser");

    fileTypeSelector->addItems(ruleDefs.fullTypeFilterPropertyList());

    qRegisterMetaType<Rule>("Rule");

    setResizeable(false);
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
    rD::fileCondition cond = sR.fieldCondition;
    rD::fileComparison comp = sR.fileCompareMode;
    if(cond == rD::filepathMode || cond == rD::extensionMode)
    {
        condWidget->setKeyWords(rulesManager::ruleKeyWordToString(sR));
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
    Q_UNUSED(event);
    emit destroyed();
}

void AbstractRuleDialog::resizeEvent(QSize newSize)
{
    Q_UNUSED(newSize);
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
        rD::fileCondition condition = sRule.fieldCondition;

        headerData << rDefs.fieldConditionToString(condition);
        headerData << rDefs.compareToString(sRule.fileCompareMode);

        if((condition == rD::dateCreatedMode || condition == rD::dateModifiedMode) &&
                sRule.fileCompareMode != rD::interval)
        {
            headerData << rulesManager::ruleKeyWordToString(sRule);
        }
        else if((condition == rD::dateCreatedMode || condition == rD::dateModifiedMode) &&
                sRule.fileCompareMode == rD::interval)
        {
            headerData << rulesManager::ruleDateLimitsToString(sRule);
        }
        else if(condition == rD::sizeMode &&
                sRule.fileCompareMode != rD::interval)
        {
           headerData << rulesManager::ruleKeyWordToString(sRule);
        }
        else if(condition == rD::sizeMode &&
                sRule.fileCompareMode == rD::interval)
        {
            headerData << rulesManager::ruleSizeLimitsToString(sRule);
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
