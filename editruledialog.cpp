#include "editruledialog.h"

EditRuleDialog::EditRuleDialog(const IRule<DefaultRuleCriteria> *editRule, QStringList watchFolders):
    AbstractRuleDialog(watchFolders)
{
    originalRuleTitle = editRule->title();
    tempRule = editRule;

    addBut->setText(tr("Færdig"));

    initializeInterface(); // Update interface according to selected rule state

    updateView(); // Update QListWidget with subrules
}

void EditRuleDialog::on_addButton_clicked()
{
    auto title = titleSelector->text();
    auto appliesTo = applySelector->currentText();
    auto action = ruleDefinitionsService()->fileActionEntityFromString(actionBox->currentText());
    auto destinations = RulesContextUtilities::splitString(pathSelector->text());
    auto typeFilter = ruleDefinitionsService()->fileTypeCriteriaFromString(fileTypeSelector->currentText());

    auto config = new DefaultRuleConfiguration();

    config->setTitle(title);
    config->setAction(action);
    config->setAppliesTo(appliesTo);
    config->setDestinations(destinations);
    config->setType(typeFilter);

    auto rule = ruleBuilderService()->buildRule(config,_ruleConditions);
    emit replaceRule(rule,originalRuleTitle);

    close();
}

void EditRuleDialog::on_addSubRule_clicked()
{

    auto currentCondition = conditionBox->currentText();
    auto criteria = ruleDefinitionsService()->buildCriteriaFromString(currentCondition);
    auto compareCriteria = condWidget->currentCompareMode();
    auto keywords = RulesContextUtilities::splitString(condWidget->keyWordValues());;
    auto sizeLimit = condWidget->fixedSizeValues();;
    auto sizeLimits = condWidget->intervalSizeValues();
    auto date = condWidget->fixedConditionalDate();
    auto dates = condWidget->intervalDates();
    auto matchWholeWords = compareCriteria == RulesContext::Match;

    auto config = new DefaultCriteriaConfiguration;

    config->setCriteria(criteria);
    config->setCompareCriteria(compareCriteria);
    config->setKeywords(keywords);
    config->setSizeLimit(sizeLimit);

    auto lowerSizeLimit = sizeLimits.first;
    auto upperSizeLimit = sizeLimits.second;

    auto lowerSizeUnits = lowerSizeLimit.first;
    auto lowerSizeDSU = lowerSizeLimit.second;

    auto upperSizeUnits = upperSizeLimit.first;
    auto upperSizeSizeDSU = upperSizeLimit.second;

    config->setSizeInterval(lowerSizeUnits,lowerSizeDSU,upperSizeUnits,upperSizeSizeDSU);

    config->setDate(date);
    config->setDates(dates);
    config->setMatchWholeWords(matchWholeWords);

    _ruleConditions << ruleBuilderService()->buildCriteria(config);
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
    actionBox->setCurrentText(ruleDefinitionsService()->fileActionEntityToString(tempRule->ruleAction()));
    titleSelector->setText(tempRule->title());
    applySelector->setCurrentText(tempRule->appliesToPath());
    pathSelector->setCurrentFilePath(RulesContextUtilities::mergeStringList(tempRule->destinationPaths()));
    fileTypeSelector->addItems(ruleDefinitionsService()->allFileTypeEntitiesToStrings());
    fileTypeSelector->setCurrentText(ruleDefinitionsService()->fileTypeCriteriaToString(tempRule->typeFilter()));
    _ruleConditions = tempRule->conditions();
}
