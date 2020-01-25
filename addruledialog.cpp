#include "addruledialog.h"

AddRuleDialog::AddRuleDialog(QStringList watchFolders):
    AbstractRuleDialog(watchFolders)
{}


void AddRuleDialog::on_treeWidget_doubleClicked(const QModelIndex &index)
{
    rD rDefs;
    int row = index.row();
    SubRule sRule = subRules.at(row);
    QString condText = rDefs.fileConditionEntityToString(sRule.criteria);
    conditionBox->setCurrentText(condText);
    conditionBox->currentTextChanged(condText);
}

void AddRuleDialog::on_addSubRule_clicked()
{
    SubRule sRule;

    rD rDefs;
    QString cText = conditionBox->currentText();
    rD::ruleCriteria criteria = rDefs.buildCriteriaFromString(cText);
    rD::ruleCompareCriteria compareCriteria = condWidget->currentCompareMode();
    sRule.criteria = rDefs.buildCriteriaFromString(cText);
    sRule.compareCriteria = condWidget->currentCompareMode();
    if(criteria == rD::fileBaseMode || criteria == rD::filepathMode|| criteria == rD::fileExtensionMode)
    {
        sRule.keyWords = Worker::splitString(condWidget->keyWordValues());

    }
    else if(criteria == rD::fileSize &&
            compareCriteria != rD::interval)
    {
        sRule.sizeLimit = condWidget->fixedSizeValues();
    }
    else if(criteria == rD::fileSize &&
            compareCriteria == rD::interval)
    {
        sRule.sizeInterval = condWidget->intervalSizeValues();
    }
    else if((criteria == rD::fileCreatedMode ||
             criteria == rD::fileModifiedMode) &&
            compareCriteria != rD::interval)
    {
        sRule.date = condWidget->fixedConditionalDate();
    }
    else if((criteria == rD::fileCreatedMode ||
             criteria == rD::fileModifiedMode) &&
            compareCriteria == rD::interval)
    {
        sRule.dateIntervals = condWidget->intervalDates();
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
    if(subRuleView->topLevelItemCount() < 1)
    {
        if(messageBox::customBox(this,tr("Advarsel"),tr("Du skal opstille nogle betingelser DIN STORE NAR!")))
        {
            return;
        }
    }

    rD rDefs;
    Rule r;
    r.title = titleSelector->text();
    r.appliesToPath = applySelector->currentText();
    r.actionRuleEntity = rDefs.fileActionEntityFromString(actionBox->currentText());
    r.destinationPaths = Worker::splitString(pathSelector->text());
    r.typeFilter = rDefs.fileTypeEntityFromString(fileTypeSelector->currentText());

    r.subRules = subRules;
    emit sendRule(r);
    close();
}
