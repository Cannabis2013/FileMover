#include "addruledialog.h"

addRuleDialog::addRuleDialog(QStringList folderPaths, QWidget *parent):
    abstractRuleDialog(folderPaths,parent)
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
    QString cText = conditionBox->currentText();
    rD::compareMode currentCompareMode = condWidget->currentCompareMode();
    sRule.fieldCondition = rD::subConditionalFromString(cText);
    sRule.fileCompareMode = condWidget->currentCompareMode();
    if(cText == "Filnavn" ||
            cText == "Filendelse")
    {
        sRule.keyWords = fW::splitString(condWidget->keyWordValues());

    }
    else if(cText == "Størrelse" &&
            currentCompareMode != rD::interval)
    {
        sRule.sizeLimit = condWidget->fixedSizeValues();
    }
    else if(cText == "Størrelse" &&
            currentCompareMode == rD::interval)
    {
        sRule.sizeIntervalLimits = condWidget->intervalSizeValues();
    }
    else if((cText == "Dato oprettet" ||
             cText == "Dato redigeret") &&
            currentCompareMode != rD::interval)
    {
        sRule.fixedDate = condWidget->fixedConditionalDate();
    }
    else if((cText == "Dato oprettet" ||
             cText == "Dato redigeret") &&
            currentCompareMode == rD::interval)
    {
        sRule.intervalDate = condWidget->intervalDates();
    }
    else if(cText == "Type")
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
