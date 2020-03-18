#include "editruledialog.h"

EditRuleDialog::EditRuleDialog(const IRule<> *editRule, QStringList watchFolders):
    AbstractRuleDialog(watchFolders)
{
    originalRuleTitle = editRule->title();
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
    auto destinations = StaticStringCollections::splitString(pathSelector->text());
    auto typeFilter = ruleService->fileTypeEntityFromString(fileTypeSelector->currentText());

    auto config = new RuleDefaultConfiguration();

    config->setTitle(title);
    config->setAction(action);
    config->setAppliesTo(appliesTo);
    config->setDestinations(destinations);
    config->setType(typeFilter);

    auto rule = ruleBuilderService()->buildOrdinaryRule(config);
    emit replaceRule(rule,originalRuleTitle);

    close();
}

void EditRuleDialog::on_addSubRule_clicked()
{

    auto currentCondition = conditionBox->currentText();
    auto criteria = ruleService->buildCriteriaFromString(currentCondition);
    auto compareCriteria = condWidget->currentCompareMode();
    auto keywords = StaticStringCollections::splitString(condWidget->keyWordValues());;
    auto sizeLimit = condWidget->fixedSizeValues();;
    auto sizeLimits = condWidget->intervalSizeValues();
    auto date = condWidget->fixedConditionalDate();
    auto dates = condWidget->intervalDates();
    auto matchWholeWords = compareCriteria == RulesContext::Match;

    auto config = new RuleConditionDefaultConfiguration;

    config->setCriteria(criteria);
    config->setCompareCriteria(compareCriteria);
    config->setKeywords(keywords);
    config->setSizeLimit(sizeLimit);
    config->setSizeInterval(sizeLimits);
    config->setDate(date);
    config->setDates(dates);
    config->setMatchWholeWords(matchWholeWords);

    auto sRule = ruleBuilderService()->buildSubRule(config);
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
    actionBox->setCurrentText(ruleService->fileActionEntityToString(tempRule->actionRuleEntity()));
    titleSelector->setText(tempRule->title());
    applySelector->setCurrentText(tempRule->appliesToPath());
    pathSelector->setCurrentFilePath(StaticStringCollections::mergeStringList(tempRule->destinationPaths()));
    fileTypeSelector->addItems(ruleService->allFileTypeEntitiesToStrings());
    fileTypeSelector->setCurrentText(ruleService->fileTypeEntityToString(tempRule->typeFilter()));
    subRules = tempRule->conditions();
}
