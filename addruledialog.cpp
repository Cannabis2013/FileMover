#include "addruledialog.h"

AddRuleDialog::AddRuleDialog(QStringList watchFolders):
    AbstractRuleDialog(watchFolders)
{
    QStringList actionList = ruleService->buildStringListFromEntity(RulesContext::Action),
            conditionList = ruleService->buildStringListFromEntity(RulesContext::Condition),
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
    auto sRule = subRules.at(row);
    QString condText = ruleService->buildStringFromCriteria(sRule->criteria());
    conditionBox->setCurrentText(condText);
    conditionBox->currentTextChanged(condText);
}

void AddRuleDialog::on_addSubRule_clicked()
{
    auto cText = conditionBox->currentText();
    auto criteria = ruleService->buildCriteriaFromString(cText);
    auto compareCriteria = condWidget->currentCompareMode();
    auto keywords = StaticStringCollections::splitString(condWidget->keyWordValues());
    auto sizeLimit = condWidget->fixedSizeValues();
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

    auto r = ruleBuilderService()->buildOrdinaryRule(config);

    emit sendRule(r);
    close();
}
