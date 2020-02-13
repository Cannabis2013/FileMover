#include "editruledialog.h"

EditRuleDialog::EditRuleDialog(const Rule editRule, QStringList watchFolders, IDefinitions *ruleService):
    AbstractRuleDialog(watchFolders, ruleService)
{
    originalRuleTitle = editRule.title;
    tempRule = editRule;

    addBut->setText(tr("Færdig"));

    initializeInterface(); // Update interface according to selected rule state
    updateView();
}

void EditRuleDialog::on_addButton_clicked()
{
    RuleDefinitions rDefs;
    tempRule.title = titleSelector->text();
    tempRule.appliesToPath = applySelector->currentText();
    tempRule.actionRuleEntity = rDefs.fileActionEntityFromString(actionBox->currentText());
    tempRule.destinationPaths = Worker::splitString(pathSelector->text());
    tempRule.subRules = subRules;
    tempRule.typeFilter = rDefs.fileTypeEntityFromString(fileTypeSelector->currentText());

    emit replaceRule(tempRule,originalRuleTitle);

    close();
}

void EditRuleDialog::on_addSubRule_clicked()
{

    auto currentCondition = conditionBox->currentText();
    auto criteria = ruleService->buildCriteriaFromString(currentCondition);
    auto currentCompareMode = condWidget->currentCompareMode();

    SubRule sRule;
    sRule.criteria = criteria;
    sRule.compareCriteria = currentCompareMode;

    if(criteria == RRT::fileBaseMode || criteria == RRT::filepathMode|| criteria == RRT::fileExtensionMode)
    {
        sRule.keyWords = Worker::splitString(condWidget->keyWordValues());
    }
    else if(criteria == RRT::fileSize &&
            currentCompareMode != RRT::interval)
    {
        sRule.sizeLimit = condWidget->fixedSizeValues();
    }
    else if(criteria == RRT::fileSize &&
            currentCompareMode == RRT::interval)
    {
        sRule.sizeInterval = condWidget->intervalSizeValues();
    }
    else if((criteria == RRT::fileCreatedMode ||
             criteria == RRT::fileModifiedMode) &&
            currentCompareMode != RRT::interval)
    {
        sRule.date = condWidget->fixedConditionalDate();
    }
    else if((criteria == RRT::fileCreatedMode ||
             criteria == RRT::fileModifiedMode) &&
            currentCompareMode == RRT::interval)
    {
        sRule.dateIntervals = condWidget->intervalDates();
    }
    subRules << sRule;
    updateView();
}

void EditRuleDialog::on_removeSubRule_clicked()
{
    int cIndex = subRuleView->currentIndex().row();
    subRules.removeAt(cIndex);
    updateView();
}

void EditRuleDialog::initializeInterface()
{
    actionBox->setCurrentText(ruleService->fileActionEntityToString(tempRule.actionRuleEntity));
    titleSelector->setText(tempRule.title);
    applySelector->setCurrentText(tempRule.appliesToPath);
    pathSelector->setCurrentFilePath(Worker::mergeStringList(tempRule.destinationPaths));
    fileTypeSelector->addItems(ruleService->allFileTypeEntitiesToStrings());
    fileTypeSelector->setCurrentText(ruleService->fileTypeEntityToString(tempRule.typeFilter));
    subRules = tempRule.subRules;
}
