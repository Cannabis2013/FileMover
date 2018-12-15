#include "editruledialog.h"

editRuleDialog::editRuleDialog(rule editRule, int index, QStringList folderPaths):
    abstractRuleDialog(editRule,folderPaths), replaceIndex(index)
{
    addBut->setText(tr("Færdig"));
    updateView();
}

void editRuleDialog::on_addButton_clicked()
{
    tempRule.title = titleSelector->text();
    tempRule.appliesToPath = applySelector->currentText();
    tempRule.actionRule = rD::actionFromString(actionBox->currentText());
    tempRule.destinationPath = fW::splitString(pathSelector->text());
    tempRule.subRules = subRules;
    tempRule.deepScanMode = deepScanRadio->isChecked();

    emit sendModifiedRule(tempRule,replaceIndex);

    close();
}

void editRuleDialog::on_addSubRule_clicked()
{
    subRule sR;
    sR.fieldCondition = rD::subConditionalFromString(conditionBox->currentText());
    sR.fileCompareMode = condWidget->currentCompareMode();

    if(conditionBox->currentText() == "Filnavn" ||
            conditionBox->currentText() == "Filendelse")
    {
        sR.keyWords = fW::splitString(condWidget->keyWordValues());

    }
    else if(conditionBox->currentText() == "Størrelse" &&
            condWidget->currentCompareMode() != rD::interval)
    {
        sR.sizeLimit = condWidget->fixedSizeValues();
    }
    else if(conditionBox->currentText() == "Størrelse" &&
            condWidget->currentCompareMode() == rD::interval)
    {
        sR.sizeIntervalLimits = condWidget->intervalSizeValues();
    }
    else if((conditionBox->currentText() == "Dato oprettet" ||
             conditionBox->currentText() == "Dato redigeret") &&
            condWidget->currentCompareMode() != rD::interval)
    {
        sR.fixedDate = condWidget->fixedConditionalDate();
    }
    else if((conditionBox->currentText() == "Dato oprettet" ||
             conditionBox->currentText() == "Dato redigeret") &&
            condWidget->currentCompareMode() == rD::interval)
    {
        sR.intervalDate = condWidget->intervalDates();
    }
    else if(conditionBox->currentText() == "Type")
    {
        sR.typeMode = condWidget->typeMode();
    }
    subRules << sR;
    updateView();
}

void editRuleDialog::on_removeSubRule_clicked()
{
    int cIndex = subRuleView->currentIndex().row();
    subRules.removeAt(cIndex);
    updateView();
}
