#include "addruledialog.h"

addRuleDialog::addRuleDialog(QStringList folderPaths, QWidget *parent):
    ruleDialog(folderPaths,parent)
{}
/*
void addRuleDialog::on_treeWidget_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    subRule sRule = subRules.at(row);
    QString condText = rD::conditionalToString(sRule.fieldCondition);
    conditionBox->setCurrentText(condText);
    conditionBox->currentTextChanged(condText);
}
*/

void addRuleDialog::on_addSubRule_clicked()
{
    subRule sRule;
    sRule.fieldCondition = rD::conditionalFromString(conditionBox->currentText());
    sRule.fileCompareMode = condWidget->currentCompareMode();
    if(conditionBox->currentText() == "Filnavn" ||
            conditionBox->currentText() == "Filendelse")
    {
        sRule.keyWords = fW::splitString(condWidget->keyWordValues());

    }
    else if(conditionBox->currentText() == "Størrelse" &&
            condWidget->currentCompareMode() != rD::interval)
    {
        sRule.sizeLimit = condWidget->fixedSizeValues();
    }
    else if(conditionBox->currentText() == "Størrelse" &&
            condWidget->currentCompareMode() == rD::interval)
    {
        sRule.sizeIntervalLimits = condWidget->intervalSizeValues();
    }
    else if((conditionBox->currentText() == "Dato oprettet" ||
             conditionBox->currentText() == "Dato redigeret") &&
            condWidget->currentCompareMode() != rD::interval)
    {
        sRule.fixedDate = condWidget->fixedConditionalDate();
    }
    else if((conditionBox->currentText() == "Dato oprettet" ||
             conditionBox->currentText() == "Dato redigeret") &&
            condWidget->currentCompareMode() == rD::interval)
    {
        sRule.intervalDate = condWidget->intervalDates();
    }
    else if(conditionBox->currentText() == "Type")
    {
        sRule.typeMode = condWidget->typeMode();
    }
    subRules << sRule;
    updateView();
}

void addRuleDialog::on_removeSubRule_clicked()
{
    int cIndex = subRuleView->currentIndex().row();
    subRules.removeAt(cIndex);
    delete subRuleView->currentItem();
}

void addRuleDialog::on_addButton_clicked()
{
    rule r;
    r.title = titleSelector->text();
    r.appliesToPath = applySelector->currentText();
    r.actionRule = rD::actionFromString(actionBox->currentText());
    r.destinationPath = fW::splitString(pathSelector->text());
    r.deepScanMode = deepScanRadio->isChecked();

    r.subRules = subRules;
    emit sendRule(r);
    close();
}
