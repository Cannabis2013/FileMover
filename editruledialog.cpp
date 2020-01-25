#include "editruledialog.h"

EditRuleDialog::EditRuleDialog(Rule editRule, QStringList watchFolders):
    AbstractRuleDialog(watchFolders)
{
    rD rDefs;
    originalRuleTitle = editRule.title;
    tempRule = editRule;
    actionBox->setCurrentText(rDefs.fileActionEntityToString(tempRule.actionRuleEntity));
    titleSelector->setText(tempRule.title);
    applySelector->setCurrentText(tempRule.appliesToPath);
    pathSelector->setCurrentFilePath(Worker::mergeStringList(tempRule.destinationPaths));
    fileTypeSelector->addItems(rDefs.allFileTypeEntitiesToStrings());
    fileTypeSelector->setCurrentText(rDefs.fileTypeEntityToString(tempRule.typeFilter));

    subRules = tempRule.subRules;

    addBut->setText(tr("Færdig"));
    updateView();
}

void EditRuleDialog::on_addButton_clicked()
{
    rD rDefs;
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
    rD rDefs;
    SubRule sRule;
    QString currentCondition = conditionBox->currentText();
    rD::ruleCriteria conMode = rDefs.buildCriteriaFromString(currentCondition);
    sRule.criteria = conMode;
    rD::ruleCompareCriteria currentCompareMode = condWidget->currentCompareMode();
    sRule.compareCriteria = currentCompareMode;
    if(conMode == rD::fileBaseMode || conMode == rD::filepathMode|| conMode == rD::fileExtensionMode)
    {
        sRule.keyWords = Worker::splitString(condWidget->keyWordValues());
    }
    else if(conMode == rD::fileSize &&
            currentCompareMode != rD::interval)
    {
        sRule.sizeLimit = condWidget->fixedSizeValues();
    }
    else if(conMode == rD::fileSize &&
            currentCompareMode == rD::interval)
    {
        sRule.sizeInterval = condWidget->intervalSizeValues();
    }
    else if((conMode == rD::fileCreatedMode ||
             conMode == rD::fileModifiedMode) &&
            currentCompareMode != rD::interval)
    {
        sRule.date = condWidget->fixedConditionalDate();
    }
    else if((conMode == rD::fileCreatedMode ||
             conMode == rD::fileModifiedMode) &&
            currentCompareMode == rD::interval)
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
