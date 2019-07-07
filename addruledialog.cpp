#include "addruledialog.h"

AddRuleDialog::AddRuleDialog(QStringList watchFolders):
    AbstractRuleDialog(watchFolders)
{}


void AddRuleDialog::on_treeWidget_doubleClicked(const QModelIndex &index)
{
    rD rDefs;
    int row = index.row();
    SubRule sRule = subRules.at(row);
    QString condText = rDefs.fieldConditionToString(sRule.fieldCondition);
    conditionBox->setCurrentText(condText);
    conditionBox->currentTextChanged(condText);
}

void AddRuleDialog::on_addSubRule_clicked()
{
    SubRule sRule;
    rD rDefs;
    QString cText = conditionBox->currentText();
    rD::fileConditionRuleEntity conMode = rDefs.fieldConditionFromString(cText);
    rD::fileCompareRuleEntity currentCompareMode = condWidget->currentCompareMode();
    sRule.fieldCondition = rDefs.fieldConditionFromString(cText);
    sRule.fileCompareMode = condWidget->currentCompareMode();
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
    rD rDefs;
    Rule r;
    r.title = titleSelector->text();
    r.appliesToPath = applySelector->currentText();
    r.actionRuleEntity = rDefs.actionFromString(actionBox->currentText());
    r.destinationPath = Worker::splitString(pathSelector->text());
    r.typeFilter = rDefs.fileTypeFromString(fileTypeSelector->currentText());
    r.deepScanMode = deepScanRadio->isChecked();

    r.subRules = subRules;
    emit sendRule(r);
    close();
}
