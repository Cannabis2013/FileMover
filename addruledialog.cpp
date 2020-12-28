#include "addruledialog.h"

AddRuleDialog::AddRuleDialog(QStringList watchFolders,IRuleDefinitions *service):
    AbstractRuleDialog(watchFolders)
{
    setRulesDefinitionsService(service);

    QStringList actionList = ruleDefinitionsService()->buildStringListFromEntity(RulesContext::Action),
            conditionList = ruleDefinitionsService()->buildStringListFromEntity(RulesContext::Criteria),
            unitList = ruleDefinitionsService()->sizeUnits();

    actionBox->addItems(actionList);
    conditionBox->addItems(conditionList);

    auto appliesToAllKeyword = ruleDefinitionsService()->defaultAppliesToAll();
    applySelector->addItem(appliesToAllKeyword);
    applySelector->addItems(watchFolders);
    applySelector->setCurrentText(appliesToAllKeyword);

    conditionBox->setCurrentText(ruleDefinitionsService()->buildDefaultStringValue());
    conditionBox->currentTextChanged(ruleDefinitionsService()->buildDefaultStringValue());
    fileTypeSelector->addItems(ruleDefinitionsService()->allFileTypeEntitiesToStrings());
}

void AddRuleDialog::on_treeWidget_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    auto condition = _ruleConditions.at(row);
    QString condText = ruleDefinitionsService()->buildStringFromCriteria(condition->criteria());
    conditionBox->setCurrentText(condText);
    conditionBox->currentTextChanged(condText);
}

void AddRuleDialog::on_addSubRule_clicked()
{
    auto cText = conditionBox->currentText();
    auto ruleCriteria = ruleDefinitionsService()->buildCriteriaFromString(cText);
    auto compareCriteria = condWidget->currentCompareMode();
    auto keywords = RulesContextUtilities::splitString(condWidget->keyWordValues());
    auto sizeLimit = condWidget->fixedSizeValues();
    auto sizeLimits = condWidget->intervalSizeValues();
    auto date = condWidget->fixedConditionalDate();
    auto dates = condWidget->intervalDates();
    auto matchWholeWords = compareCriteria == RulesContext::Match;

    auto config = new DefaultCriteriaConfiguration;

    config->setCriteria(ruleCriteria);
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

    auto criteria = ruleBuilderService()->buildCriteria(config);
    _ruleConditions << criteria;
    updateView();
}

void AddRuleDialog::on_removeSubRule_clicked()
{
    int cIndex = subRuleView->currentIndex().row();
    _ruleConditions.removeAt(cIndex);
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

    auto r = ruleBuilderService()->buildRule(config,_ruleConditions);

    emit sendRule(r);
    close();
}
