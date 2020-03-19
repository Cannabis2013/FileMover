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

    applySelector->addItems(watchFolders);

    qRegisterMetaType<IRule<IDefaultRuleCondition>*>("IRule<IDefaultRuleCondition>");

    setResizeable(false);
}

AbstractRuleDialog::~AbstractRuleDialog()
{
    delete ui;
    delete this;
}

void AbstractRuleDialog::setRulesDefinitionsService(IRuleDefinitions *service)
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

void AbstractRuleDialog::updateConditionView(const IDefaultRuleCondition *sR)
{
    auto cond = sR->criteria();
    auto comp = sR->compareCriteria();

    if(cond == DefaultRulesContext::FileSizeMode && comp != DefaultRulesContext::Interval)
    {
        condWidget->setConditionalFixedSize(sR->sizeLimit(),sR->compareCriteria());
    }
    else if(cond == DefaultRulesContext::FileSizeMode && comp== DefaultRulesContext::Interval)
    {
        condWidget->setConditionalIntervalSize(sR->sizeInterval());
    }
    else if((cond == DefaultRulesContext::FileCreatedMode || cond == DefaultRulesContext::FileModifiedMode) &&
            comp != DefaultRulesContext::Interval)
    {
        condWidget->setFixedDate(sR->date());
        condWidget->setCompareView(sR->compareCriteria());
    }
    else if((cond == DefaultRulesContext::FileCreatedMode || cond == DefaultRulesContext::FileModifiedMode) &&
            comp == DefaultRulesContext::Interval)
    {
        condWidget->setIntervalDate(sR->dateIntervals());
    }
    else
    {
        condWidget->setKeyWords(DefaultRulesContext::ruleKeyWordToString(sR));
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
    conditionBox->setCurrentText(ruleDefinitionsService()->buildStringFromCriteria(clickedSubRule->criteria()));
    conditionBox->currentTextChanged(ruleDefinitionsService()->buildStringFromCriteria( clickedSubRule->criteria()));

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
        auto criteria = subRules.at(i);
        auto condition = criteria->criteria();

        headerData << ruleDefinitionsService()->buildStringFromCriteria(condition);
        headerData << ruleDefinitionsService()->buildStringFromCompareCriteria(criteria->compareCriteria());

        if((condition == DefaultRulesContext::FileCreatedMode || condition == DefaultRulesContext::FileModifiedMode) &&
                criteria->compareCriteria() != DefaultRulesContext::Interval)
        {
            headerData << DefaultRulesContext::ruleKeyWordToString(criteria);
        }
        else if((condition == DefaultRulesContext::FileCreatedMode || condition == DefaultRulesContext::FileModifiedMode) &&
                criteria->compareCriteria() == DefaultRulesContext::Interval)
        {
            headerData << DefaultRulesContext::ruleDateLimitsToString(criteria);
        }
        else if(condition == DefaultRulesContext::FileSizeMode &&
                criteria->compareCriteria() != DefaultRulesContext::Interval)
        {
           headerData << DefaultRulesContext::ruleKeyWordToString(criteria);
        }
        else if(condition == DefaultRulesContext::FileSizeMode &&
                criteria->compareCriteria() == DefaultRulesContext::Interval)
        {
            headerData << DefaultRulesContext::ruleSizeLimitsToString(criteria);
        }
        else
        {
            headerData << StaticStringCollections::mergeStringList(criteria->keyWords());
        }

        new QTreeWidgetItem(subRuleView,headerData);
    }
}

void AbstractRuleDialog::on_actionComboBox_currentIndexChanged(const QString &arg1)
{
    (ruleDefinitionsService()->fileActionEntityFromString(arg1) == DefaultRulesContext::DeleteAction) ?
                destinationFrame->hide() :
                destinationFrame->show();
}
