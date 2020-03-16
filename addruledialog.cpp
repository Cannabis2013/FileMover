#include "addruledialog.h"

AddRuleDialog::AddRuleDialog(QStringList watchFolders, IRuleDefinitions<RulesContext::RuleType,
                             RulesContext::RuleAction,
                             RulesContext::RuleCriteria,
                             RulesContext::RuleCompareCriteria,
                             RulesContext::FileType> *service):
    AbstractRuleDialog(watchFolders, service)
{
    QStringList actionList = ruleService->buildStringListFromEntity(RulesContext::Action),
            conditionList = ruleService->buildStringListFromEntity(RulesContext::Criteria),
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
    QString condText = ruleService->buildStringFromCriteria(sRule.criteria());
    conditionBox->setCurrentText(condText);
    conditionBox->currentTextChanged(condText);
}

void AddRuleDialog::on_addSubRule_clicked()
{
    auto cText = conditionBox->currentText();
    auto criteria = ruleService->buildCriteriaFromString(cText);
    auto compareCriteria = condWidget->currentCompareMode();
    auto keyWords = StaticStringCollections::splitString(condWidget->keyWordValues());
    auto sizeLimit = condWidget->fixedSizeValues();
    auto sizeLimits = condWidget->intervalSizeValues();
    auto date = condWidget->fixedConditionalDate();
    auto dates = condWidget->intervalDates();
    auto matchWholeWords = compareCriteria == RulesContext::match;


    auto sRule = RuleBuilder::buildSubRule(criteria,
                                           compareCriteria,
                                           keyWords,
                                           sizeLimit,
                                           date,
                                           sizeLimits,
                                           dates,
                                           matchWholeWords);

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

    auto r = RuleBuilder::buildOrdinaryRule(title,appliesTo,destinations,action,typeFilter,subRules);
    emit sendRule(r);
    close();
}
