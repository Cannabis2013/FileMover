#include "editruledialog.h"

EditRuleDialog::EditRuleDialog(const IRule<IDefaultRuleCondition> *editRule, QStringList watchFolders):
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
    auto action = ruleDefinitionsService()->fileActionEntityFromString(actionBox->currentText());
    auto destinations = RulesContext::splitString(pathSelector->text());
    auto typeFilter = ruleDefinitionsService()->fileTypeCriteriaFromString(fileTypeSelector->currentText());

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
    auto criteria = ruleDefinitionsService()->buildCriteriaFromString(currentCondition);
    auto compareCriteria = condWidget->currentCompareMode();
    auto keywords = RulesContext::splitString(condWidget->keyWordValues());;
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

    auto condition = ruleBuilderService()->buildSubRule(config);
    _ruleConditions << condition;
    updateView();
}

void EditRuleDialog::on_removeSubRule_clicked()
{
    int cIndex = subRuleView->currentIndex().row();
    _ruleConditions.removeAt(cIndex);
    updateView();
}

void EditRuleDialog::initializeInterface()
{
    actionBox->setCurrentText(ruleDefinitionsService()->fileActionEntityToString(tempRule->actionRuleEntity()));
    titleSelector->setText(tempRule->title());
    applySelector->setCurrentText(tempRule->appliesToPath());
    pathSelector->setCurrentFilePath(RulesContext::mergeStringList(tempRule->destinationPaths()));
    fileTypeSelector->addItems(ruleDefinitionsService()->allFileTypeEntitiesToStrings());
    fileTypeSelector->setCurrentText(ruleDefinitionsService()->fileTypeCriteriaToString(tempRule->typeFilter()));
    _ruleConditions = tempRule->conditions();
}
