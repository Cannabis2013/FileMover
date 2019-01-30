#include "editruledialog.h"

editRuleDialog::editRuleDialog(rule editRule, int index, QStringList folderPaths):
    abstractRuleDialog(editRule,folderPaths), replaceIndex(index)
{
    addBut->setText(tr("Færdig"));
    updateView();
}

void editRuleDialog::on_addButton_clicked()
{
    rD rDefs;
    tempRule.title = titleSelector->text();
    tempRule.appliesToPath = applySelector->currentText();
    tempRule.actionRule = rDefs.actionFromString(actionBox->currentText());
    tempRule.destinationPath = fW::splitString(pathSelector->text());
    tempRule.subRules = subRules;
    tempRule.deepScanMode = deepScanRadio->isChecked();

    emit sendModifiedRule(tempRule,replaceIndex);

    close();
}

void editRuleDialog::on_addSubRule_clicked()
{
    rD rDefs;
    subRule sRule;
    QString currentCondition = conditionBox->currentText();
    rD::fileFieldCondition conMode = rDefs.fieldConditionFromString(currentCondition);
    sRule.fieldCondition = conMode;
    rD::compareMode currentCompareMode = condWidget->currentCompareMode();
    sRule.fileCompareMode = currentCompareMode;
    if(conMode == rD::filepathMode|| conMode == rD::extensionMode)
    {
        sRule.keyWords = fW::splitString(condWidget->keyWordValues());

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

void editRuleDialog::on_removeSubRule_clicked()
{
    int cIndex = subRuleView->currentIndex().row();
    subRules.removeAt(cIndex);
    updateView();
}
