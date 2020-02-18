#include "editruledialog.h"

EditRuleDialog::EditRuleDialog(const Rule editRule, QStringList watchFolders, IDefinitions *ruleService):
    AbstractRuleDialog(watchFolders, ruleService)
{
    originalRuleTitle = editRule.title();
    tempRule = editRule;

    addBut->setText(tr("Færdig"));

    initializeInterface(); // Update interface according to selected rule state
    updateView();
}

void EditRuleDialog::on_addButton_clicked()
{
    auto title = titleSelector->text();
    auto appliesTo = applySelector->currentText();
    auto action = ruleService->fileActionEntityFromString(actionBox->currentText());
    auto destinations = Worker::splitString(pathSelector->text());

    auto typeFilter = ruleService->fileTypeEntityFromString(fileTypeSelector->currentText());

    Rule r = RuleBuilder::buildOrdinaryRule(title,appliesTo,destinations,action,typeFilter,subRules);

    emit replaceRule(r,originalRuleTitle);

    close();
}

void EditRuleDialog::on_addSubRule_clicked()
{

    auto currentCondition = conditionBox->currentText();
    auto criteria = ruleService->buildCriteriaFromString(currentCondition);
    auto currentCompareMode = condWidget->currentCompareMode();
    auto keyWords = Worker::splitString(condWidget->keyWordValues());;
    auto sizeLimit = condWidget->fixedSizeValues();;
    auto sizeLimits = condWidget->intervalSizeValues();
    auto date = condWidget->fixedConditionalDate();
    auto dates = condWidget->intervalDates();
    auto matchWholeWords = currentCompareMode == RRT::match;

    auto sRule = RuleBuilder::buildSubRule(criteria,
                                           currentCompareMode,
                                           keyWords,
                                           sizeLimit,
                                           date,
                                           sizeLimits,
                                           dates,
                                           matchWholeWords);

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
    actionBox->setCurrentText(ruleService->fileActionEntityToString(tempRule.actionRuleEntity()));
    titleSelector->setText(tempRule.title());
    applySelector->setCurrentText(tempRule.appliesToPath());
    pathSelector->setCurrentFilePath(Worker::mergeStringList(tempRule.destinationPaths()));
    fileTypeSelector->addItems(ruleService->allFileTypeEntitiesToStrings());
    fileTypeSelector->setCurrentText(ruleService->fileTypeEntityToString(tempRule.typeFilter()));
    subRules = tempRule.subRules();
}
