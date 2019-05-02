#include "editruledialog.h"

EditRuleDialog::EditRuleDialog(Rule editRule, QStringList watchFolders):
    AbstractRuleDialog(watchFolders)
{
    originalRuleTitle = editRule.title;
    tempRule = editRule;
    actionBox->setCurrentText(ruleDefs.actionToString(tempRule.actionRule));
    titleSelector->setText(tempRule.title);
    applySelector->setCurrentText(tempRule.appliesToPath);
    pathSelector->setLineText(Worker::mergeStringList(tempRule.destinationPath));
    deepScanRadio->setChecked(tempRule.deepScanMode);

    subRules = tempRule.subRules;

    addBut->setText(tr("Færdig"));
    updateView();
}

void EditRuleDialog::on_addButton_clicked()
{
    rD rDefs;
    tempRule.title = titleSelector->text();
    tempRule.appliesToPath = applySelector->currentText();
    tempRule.actionRule = rDefs.actionFromString(actionBox->currentText());
    tempRule.destinationPath = Worker::splitString(pathSelector->text());
    tempRule.subRules = subRules;
    tempRule.deepScanMode = deepScanRadio->isChecked();

    emit replaceRule(tempRule,originalRuleTitle);

    close();
}

void EditRuleDialog::on_addSubRule_clicked()
{
    rD rDefs;
    SubRule sRule;
    QString currentCondition = conditionBox->currentText();
    rD::fileFieldCondition conMode = rDefs.fieldConditionFromString(currentCondition);
    sRule.fieldCondition = conMode;
    rD::compareMode currentCompareMode = condWidget->currentCompareMode();
    sRule.fileCompareMode = currentCompareMode;
    if(conMode == rD::filepathMode|| conMode == rD::extensionMode)
    {
        sRule.keyWords = Worker::splitString(condWidget->keyWordValues());

    }
    else if(conMode == rD::sizeMode &&
            currentCompareMode != rD::interval)
    {
        sRule.sizeLimit = condWidget->fixedSizeValues();
    }
    else if(conMode == rD::sizeMode &&
            currentCompareMode == rD::interval)
    {
        sRule.sizeIntervalLimits = condWidget->intervalSizeValues();
    }
    else if((conMode == rD::dateCreatedMode ||
             conMode == rD::dateModifiedMode) &&
            currentCompareMode != rD::interval)
    {
        sRule.fixedDate = condWidget->fixedConditionalDate();
    }
    else if((conMode == rD::dateCreatedMode ||
             conMode == rD::dateModifiedMode) &&
            currentCompareMode == rD::interval)
    {
        sRule.intervalDate = condWidget->intervalDates();
    }
    else if(conMode == rD::typeMode)
    {
        sRule.typeMode = condWidget->typeMode();
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
