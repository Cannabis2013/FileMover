#include "abstractruledialog.h"
#include "ui_abstractruledialog.h"

AbstractRuleDialog::AbstractRuleDialog(QStringList watchFolders) :
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


    qRegisterMetaType<IRule<>*>("IRule");

    setResizeable(false);
}

AbstractRuleDialog::~AbstractRuleDialog()
{
    delete ui;
    delete this;
}

void AbstractRuleDialog::setRulesDefinitionsService(IDefinitions *service)
{
    RuleServicesInjector::setRulesDefinitionsService(service);
    condWidget->setRulesDefinitionsService(service);
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

void AbstractRuleDialog::updateConditionView(const IRuleCondition *sR)
{
    RulesContext::RuleCriteria cond = sR->criteria();
    RulesContext::RuleCompareCriteria comp = sR->compareCriteria();

    if(cond == RulesContext::FileSize && comp != RulesContext::Interval)
    {
        condWidget->setConditionalFixedSize(sR->sizeLimit(),sR->compareCriteria());
    }
    else if(cond == RulesContext::FileSize && comp== RulesContext::Interval)
    {
        condWidget->setConditionalIntervalSize(sR->sizeInterval());
    }
    else if((cond == RulesContext::FileCreatedMode || cond == RulesContext::FileModifiedMode) &&
            comp != RulesContext::Interval)
    {
        condWidget->setFixedDate(sR->date());
        condWidget->setCompareView(sR->compareCriteria());
    }
    else if((cond == RulesContext::FileCreatedMode || cond == RulesContext::FileModifiedMode) &&
            comp == RulesContext::Interval)
    {
        condWidget->setIntervalDate(sR->dateIntervals());
    }
    else
    {
        condWidget->setKeyWords(RulesContext::ruleKeyWordToString(sR));
        condWidget->setCompareView(sR->compareCriteria());
    }
}

void AbstractRuleDialog::on_conditionComboBox_currentIndexChanged(const QString &arg1)
{
    condWidget->setMode(arg1);
}

void AbstractRuleDialog::on_treeWidget_doubleClicked(const QModelIndex &index)
{
    int rowIndex = index.row();
    auto clickedSubRule = subRules.at(rowIndex);
    conditionBox->setCurrentText(ruleService->buildStringFromCriteria(clickedSubRule->criteria()));
    conditionBox->currentTextChanged(ruleService->buildStringFromCriteria( clickedSubRule->criteria()));

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
        auto sRule = subRules.at(i);
        RulesContext::RuleCriteria condition = sRule->criteria();

        headerData << ruleService->buildStringFromCriteria(condition);
        headerData << ruleService->buildStringFromCompareCriteria(sRule->compareCriteria());

        if((condition == RulesContext::FileCreatedMode || condition == RulesContext::FileModifiedMode) &&
                sRule->compareCriteria() != RulesContext::Interval)
        {
            headerData << RulesContext::ruleKeyWordToString(sRule);
        }
        else if((condition == RulesContext::FileCreatedMode || condition == RulesContext::FileModifiedMode) &&
                sRule->compareCriteria() == RulesContext::Interval)
        {
            headerData << RulesContext::ruleDateLimitsToString(sRule);
        }
        else if(condition == RulesContext::FileSize &&
                sRule->compareCriteria() != RulesContext::Interval)
        {
           headerData << RulesContext::ruleKeyWordToString(sRule);
        }
        else if(condition == RulesContext::FileSize &&
                sRule->compareCriteria() == RulesContext::Interval)
        {
            headerData << RulesContext::ruleSizeLimitsToString(sRule);
        }
        else
        {
            headerData << StaticStringCollections::mergeStringList(sRule->keyWords());
        }

        new QTreeWidgetItem(subRuleView,headerData);
    }
}

void AbstractRuleDialog::on_actionComboBox_currentIndexChanged(const QString &arg1)
{
    (ruleService->fileActionEntityFromString(arg1) == RulesContext::DeleteAction) ?
                destinationFrame->hide() :
                destinationFrame->show();
}
