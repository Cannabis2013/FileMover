#include "addruledialog.h"

AddRuleDialog::AddRuleDialog(QStringList watchFolders, IRuleDefinitions<RRT::RuleType,
                             RRT::RuleAction,
                             RRT::RuleCriteria,
                             RRT::RuleCompareCriteria,
                             RRT::FileTypeEntity> *service):
    AbstractRuleDialog(watchFolders, service)
{
    QStringList actionList = ruleService->buildStringListFromEntity(RRT::action),
            conditionList = ruleService->buildStringListFromEntity(RRT::criteria),
            unitList = ruleService->sizeUnits();

    actionBox->addItems(actionList);
    conditionBox->addItems(conditionList);

    applySelector->addItem( "Alle");
    applySelector->addItems(watchFolders);
    applySelector->setCurrentText("Alle");

    conditionBox->setCurrentText(ruleService->buildDefaultStringValue());
    conditionBox->currentTextChanged(ruleService->buildDefaultStringValue());
    fileTypeSelector->addItems(ruleService->allFileTypeEntitiesToStrings());
}


void AddRuleDialog::on_treeWidget_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    SubRule sRule = subRules.at(row);
    QString condText = ruleService->buildStringFromCriteria(sRule.criteria);
    conditionBox->setCurrentText(condText);
    conditionBox->currentTextChanged(condText);
}

void AddRuleDialog::on_addSubRule_clicked()
{
    SubRule sRule;

    QString cText = conditionBox->currentText();
    RRT::RuleCriteria criteria = ruleService->buildCriteriaFromString(cText);
    RRT::RuleCompareCriteria compareCriteria = condWidget->currentCompareMode();
    sRule.criteria = ruleService->buildCriteriaFromString(cText);
    sRule.compareCriteria = condWidget->currentCompareMode();
    if(criteria == RRT::fileBaseMode || criteria == RRT::filepathMode|| criteria == RRT::fileExtensionMode)
    {
        sRule.keyWords = Worker::splitString(condWidget->keyWordValues());

    }
    else if(criteria == RRT::fileSize &&
            compareCriteria != RRT::interval)
    {
        sRule.sizeLimit = condWidget->fixedSizeValues();
    }
    else if(criteria == RRT::fileSize &&
            compareCriteria == RRT::interval)
    {
        sRule.sizeInterval = condWidget->intervalSizeValues();
    }
    else if((criteria == RRT::fileCreatedMode ||
             criteria == RRT::fileModifiedMode) &&
            compareCriteria != RRT::interval)
    {
        sRule.date = condWidget->fixedConditionalDate();
    }
    else if((criteria == RRT::fileCreatedMode ||
             criteria == RRT::fileModifiedMode) &&
            compareCriteria == RRT::interval)
    {
        sRule.dateIntervals = condWidget->intervalDates();
    }
    subRules << sRule;
    updateView();
}

void AddRuleDialog::on_removeSubRule_clicked()
{
    int cIndex = subRuleView->currentIndex().row();
    subRules.removeAt(cIndex);
    delete subRuleView->currentItem();
}


void AddRuleDialog::on_addButton_clicked()
{
    if(subRuleView->topLevelItemCount() < 1)
    {
        if(messageBox::customBox(this,tr("Advarsel"),tr("Du skal opstille nogle betingelser DIN STORE NAR!")))
        {
            return;
        }
    }

    RuleDefinitions rDefs;
    Rule r;
    r.title = titleSelector->text();
    r.appliesToPath = applySelector->currentText();
    r.actionRuleEntity = rDefs.fileActionEntityFromString(actionBox->currentText());
    r.destinationPaths = Worker::splitString(pathSelector->text());
    r.typeFilter = rDefs.fileTypeEntityFromString(fileTypeSelector->currentText());

    r.subRules = subRules;
    emit sendRule(r);
    close();
}
