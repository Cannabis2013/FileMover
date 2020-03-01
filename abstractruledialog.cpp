#include "abstractruledialog.h"
#include "ui_abstractruledialog.h"

AbstractRuleDialog::AbstractRuleDialog(QStringList watchFolders,
                                       IDefinitions *rService) :
    AbstractFrame(),
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
    condWidget = ui->conditionView;
    ruleService = rService;

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
    RRT::RuleCriteria cond = sR.criteria();
    RRT::RuleCompareCriteria comp = sR.compareCriteria();

    if(cond == RRT::fileSize && comp != RRT::interval)
    {
        condWidget->setConditionalFixedSize(sR.sizeLimit(),sR.compareCriteria());
    }
    else if(cond == RRT::fileSize && comp== RRT::interval)
    {
        condWidget->setConditionalIntervalSize(sR.sizeInterval());
    }
    else if((cond == RRT::fileCreatedMode || cond == RRT::fileModifiedMode) &&
            comp != RRT::interval)
    {
        condWidget->setFixedDate(sR.date());
        condWidget->setCompareView(sR.compareCriteria());
    }
    else if((cond == RRT::fileCreatedMode || cond == RRT::fileModifiedMode) &&
            comp == RRT::interval)
    {
        condWidget->setIntervalDate(sR.dateIntervals());
    }
    else
    {
        condWidget->setKeyWords(StaticStringCollections::ruleKeyWordToString(sR));
        condWidget->setCompareView(sR.compareCriteria());
    }
}

void AbstractRuleDialog::on_conditionComboBox_currentIndexChanged(const QString &arg1)
{
    condWidget->setMode(arg1);
}

void AbstractRuleDialog::on_treeWidget_doubleClicked(const QModelIndex &index)
{
    int rowIndex = index.row();
    RuleDefinitions rDefs;
    SubRule clickedSubRule = subRules.at(rowIndex);
    conditionBox->setCurrentText(rDefs.buildStringFromCriteria(clickedSubRule.criteria()));
    conditionBox->currentTextChanged(rDefs.buildStringFromCriteria( clickedSubRule.criteria()));

    updateConditionView(clickedSubRule);
}

void AbstractRuleDialog::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    emit destroyed();
}

void AbstractRuleDialog::resizeNotify(QSize newSize)
{
    Q_UNUSED(newSize)
}


void AbstractRuleDialog::updateView()
{
    subRuleView->clear();
    int total = subRules.count();
    for (int i = 0; i < total; ++i)
    {
        QStringList headerData;
        SubRule sRule = subRules.at(i);
        RRT::RuleCriteria condition = sRule.criteria();

        headerData << ruleService->buildStringFromCriteria(condition);
        headerData << ruleService->buildStringFromCompareCriteria(sRule.compareCriteria());

        if((condition == RRT::fileCreatedMode || condition == RRT::fileModifiedMode) &&
                sRule.compareCriteria() != RRT::interval)
        {
            headerData << StaticStringCollections::ruleKeyWordToString(sRule);
        }
        else if((condition == RRT::fileCreatedMode || condition == RRT::fileModifiedMode) &&
                sRule.compareCriteria() == RRT::interval)
        {
            headerData << StaticStringCollections::ruleDateLimitsToString(sRule);
        }
        else if(condition == RRT::fileSize &&
                sRule.compareCriteria() != RRT::interval)
        {
           headerData << StaticStringCollections::ruleKeyWordToString(sRule);
        }
        else if(condition == RRT::fileSize &&
                sRule.compareCriteria() == RRT::interval)
        {
            headerData << StaticStringCollections::ruleSizeLimitsToString(sRule);
        }
        else
        {
            headerData << StaticStringCollections::mergeStringList(sRule.keyWords());
        }

        new QTreeWidgetItem(subRuleView,headerData);
    }
}

void AbstractRuleDialog::on_actionComboBox_currentIndexChanged(const QString &arg1)
{
    RuleDefinitions rDefs;
    (rDefs.fileActionEntityFromString(arg1) == RRT::Delete) ?
                destinationFrame->hide() :
                destinationFrame->show();
}
