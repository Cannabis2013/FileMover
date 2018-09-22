#include "conditionWidget.h"
#include "ui_conditionWidget.h"

conditionWidget::conditionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::conditionWidget)
{
    ui->setupUi(this);

    mainModeSelector = ui->modeSelector;
    keyWordSelector = ui->keyWordSelector;

    // Type related..

    folderCheckBox = ui->folderSelector;
    fileCheckBox = ui->fileSelector;

    // Size related..
    fixedSizeSelector = ui->fixedSizeSelector;
    fixedSizeModeSelector = ui->fixedSizeOperatorSelector;
    minIntervalSizeSelector = ui->sizeIntervalMinSelector;
    maxIntervalSizeSelector = ui->sizeIntervalMaxSelector;

    // Date related..

    dateOperatorSelector = ui->dateFixedOperatorSelector;
    ui->dateFixedSelector->setDate(QDate::currentDate());

    //  Units..
    fixedSizeUnitSelector = ui->fixedSizeUnitSelector;
    sizeMinIntervalUnitSelector = ui->sizeIntervalMinUnit;
    sizeMaxIntervalUnitSelector = ui->sizeIntervalMaxUnit;

    initDefaultOperators();
    initDefaultUnits();

    mainModeView = ui->modeWidget;

    mainModeView->setCurrentIndex(4);
}

conditionWidget::~conditionWidget()
{
    delete ui;
}

void conditionWidget::setKeyWords(QString kW)
{
    ui->keyWordSelector->setText(kW);
}

void conditionWidget::setConditionalFixedSize(QPair<int, QString> fSize, ruleDefinitions::compareMode cMode)
{
    rD rDefs;
    ui->modeSelector->setCurrentText(rDefs.intervalConditionalList().at(0));

    ui->fixedSizeSelector->setValue(fSize.first);
    ui->fixedSizeUnitSelector->setCurrentText(fSize.second);
    if(cMode == rD::bigger)
        ui->fixedSizeOperatorSelector->setCurrentText("Større end");
    else if(cMode == rD::biggerOrEqual)
        ui->fixedSizeOperatorSelector->setCurrentText("Større eller lig med");
    else if(cMode == rD::equal)
        ui->fixedSizeOperatorSelector->setCurrentText("Lig med");
    else if(cMode == rD::lesserOrEqual)
        ui->fixedSizeOperatorSelector->setCurrentText("Mindre eller lig med");
    else if(cMode == rD::lesser)
        ui->fixedSizeOperatorSelector->setCurrentText("Mindre end");
}

void conditionWidget::setConditionalIntervalSize(QPair<QPair<int, QString>, QPair<int, QString> > iSize)
{
    rD rDefs;
    ui->modeSelector->setCurrentText(rDefs.intervalConditionalList().at(1));

    QPair<int,QString> minSize = iSize.first;
    QPair<int,QString> maxSize = iSize.second;

    ui->sizeIntervalMinSelector->setValue(minSize.first);
    ui->sizeIntervalMinUnit->setCurrentText(minSize.second);

    ui->sizeIntervalMaxSelector->setValue(maxSize.first);
    ui->sizeIntervalMaxUnit->setCurrentText(maxSize.second);
}

void conditionWidget::setFixedDate(QPair<ruleDefinitions::compareMode, myDateTime> dt)
{
    rD::compareMode cM = dt.first;
    myDateTime dateTime = dt.second;

    rD rDefs;
    ui->modeSelector->setCurrentText(rDefs.intervalConditionalList().at(0));

    ui->dateFixedSelector->setDate(dateTime.date());
    dateOperatorSelector->setCurrentText(rD::compareModeToString(cM));
}

void conditionWidget::setIntervalDate(QPair<myDateTime, myDateTime> iDate)
{
    rD rDefs;
    ui->modeSelector->setCurrentText(rDefs.intervalConditionalList().at(1));

    ui->dateMinSelector->setDate(iDate.first.date());
    ui->dateMaxSelector->setDate(iDate.second.date());
}

void conditionWidget::setTypeValues(baseWorker::iteratorMode tMode)
{
    if(tMode == fW::folderOnly)
    {
        folderCheckBox->setChecked(true);
        fileCheckBox->setChecked(false);
    }
    else if(tMode == fW::filesOnly)
    {
        fileCheckBox->setChecked(true);
        folderCheckBox->setChecked(false);
    }
    else if(tMode == fW::allEntries)
    {
        fileCheckBox->setChecked(true);
        folderCheckBox->setChecked(true);
    }
}

void conditionWidget::setCurrentView(QString txt)
{
    currentFileModeRule = rD::conditionalFromString(txt);
    mainModeSelector->show();

    if(txt == "Filnavn" || txt == "Filendelse")
        mainModeView->setCurrentIndex(0);
    else if(txt == "Størrelse")
        mainModeView->setCurrentIndex(1);
    else if(txt == "Dato oprettet" || txt == "Dato redigeret")
        mainModeView->setCurrentIndex(2);
    else if(txt == "Type")
        mainModeView->setCurrentIndex(3);
    else
    {
        mainModeSelector->hide();
        mainModeView->setCurrentIndex(4);
    }
}

void conditionWidget::setCurrentView(int index)
{
    mainModeView->setCurrentIndex(index);
}

void conditionWidget::setCompareView(ruleDefinitions::compareMode compare)
{
    if(currentFileModeRule == rD::filepathMode || currentFileModeRule == rD::extensionMode)
    {
        mainModeSelector->setCurrentText(rD::compareModeToString(compare));
    }
    else if(currentFileModeRule == rD::sizeMode && currentCompareMode() != rD::interval)
    {
        fixedSizeModeSelector->setCurrentText(rD::compareModeToString(compare));
    }
    else if((currentFileModeRule == rD::dateCreatedMode ||
             currentFileModeRule == rD::dateModifiedMode) &&
            currentCompareMode() != rD::interval)
    {
        dateOperatorSelector->setCurrentText(rD::compareModeToString(compare));
    }
}

void conditionWidget::initDefaultOperators()
{
    fixedSizeModeSelector->clear();
    dateOperatorSelector->clear();

    rD rDefs;

    fixedSizeModeSelector->addItems(rDefs.sizeOperators);
    dateOperatorSelector->addItems(rDefs.dateOperators);
}

void conditionWidget::initDefaultUnits()
{
    fixedSizeUnitSelector->clear();
    sizeMinIntervalUnitSelector->clear();
    sizeMaxIntervalUnitSelector->clear();

    rD rDefs;
    fixedSizeUnitSelector->addItems(rDefs.sizeUnits());
    sizeMinIntervalUnitSelector->addItems(rDefs.sizeUnits());
    sizeMaxIntervalUnitSelector->addItems(rDefs.sizeUnits());
}

void conditionWidget::resetValues()
{
    keyWordSelector->clear();
    ui->sizeIntervalMaxSelector->clear();
    ui->sizeIntervalMinSelector->clear();

    ui->fixedSizeSelector->clear();

    ui->dateFixedSelector->clear();

    ui->dateMinSelector->clear();
    ui->dateMaxSelector->clear();

    ui->fileSelector->setChecked(false);
    ui->folderSelector->setChecked(false);

    initDefaultOperators();
    initDefaultUnits();
}

void conditionWidget::setMode(QString condition)
{
    mainModeSelector->clear();
    currentFileModeRule = rD::conditionalFromString(condition);

    setCurrentView(condition);

    rD rDefs;
    if(condition == "Filnavn" || condition == "Filendelse")
    {
        mainModeSelector->addItems(rDefs.fileNameCompareModeList);
    }
    else if(condition == "Størrelse")
    {
        mainModeSelector->addItems(rDefs.intervalConditionalList());
        ui->fixedSizeOperatorSelector->clear();
        ui->fixedSizeOperatorSelector->addItems(rDefs.sizeOperators);
    }
    else if(condition == "Dato oprettet" || condition == "Dato redigeret")
        mainModeSelector->addItems(rDefs.intervalConditionalList());
    else if(condition == "Type")
        mainModeSelector->addItem(condition);
}

ruleDefinitions::compareMode conditionWidget::currentCompareMode()
{
    rD rDefs;
    QString modeText = ui->modeSelector->currentText();

    if(currentFileModeRule == rD::filepathMode || currentFileModeRule == rD::extensionMode)
        return rD::compareModeFromString(modeText);
    else if(currentFileModeRule == rD::sizeMode && modeText == rDefs.intervalConditionalList().at(0))
        return rD::compareModeFromString(ui->fixedSizeOperatorSelector->currentText());
    else if(currentFileModeRule == rD::sizeMode && modeText == rDefs.intervalConditionalList().at(1))
        return rD::interval;
    else if((currentFileModeRule == rD::dateCreatedMode ||
             currentFileModeRule == rD::dateModifiedMode) &&
            modeText == rDefs.intervalConditionalList().at(0))
        return rD::compareModeFromString(dateOperatorSelector->currentText());
    else if((currentFileModeRule == rD::dateCreatedMode ||
             currentFileModeRule == rD::dateModifiedMode) &&
            modeText == rDefs.intervalConditionalList().at(1))
        return rD::interval;
    else
        return rD::noCompareModeSet;
}

QString conditionWidget::keyWordValues() const
{
    return ui->keyWordSelector->text();
}

QPair<int, QString> conditionWidget::fixedSizeValues() const
{
    QPair<int,QString> result;
    int val = fixedSizeSelector->value();
    QString unit = fixedSizeUnitSelector->currentText();

    result.first = val;
    result.second = unit;

    return result;
}

QPair<QPair<int, QString>, QPair<int, QString> > conditionWidget::intervalSizeValues() const
{
    QPair<QPair<int,QString>,QPair<int,QString>> result;

    QPair<int,QString>minValues;
    QPair<int,QString>maxValues;

    int val = minIntervalSizeSelector->value();
    QString unit = sizeMinIntervalUnitSelector->currentText();

    minValues.first = val;
    minValues.second = unit;

    val = maxIntervalSizeSelector->value();
    unit = sizeMaxIntervalUnitSelector->currentText();

    maxValues.first = val;
    maxValues.second = unit;

    result.first = minValues;
    result.second = maxValues;

    return result;
}

QPair<ruleDefinitions::compareMode, myDateTime> conditionWidget::fixedConditionalDate() const
{
    QPair<ruleDefinitions::compareMode, myDateTime> result;
    QString dateOperator = dateOperatorSelector->currentText();
    rD::compareMode compareOperator =rD::compareModeFromString(dateOperator);

    QDateTime dt = ui->dateFixedSelector->dateTime();
    myDateTime mDate(dt);
    result.first = compareOperator;
    result.second = mDate;

    return result;
}

QPair<myDateTime, myDateTime> conditionWidget::intervalDates() const
{
    QPair<myDateTime, myDateTime>result;
    myDateTime minDate(ui->dateMinSelector->dateTime()), maxDate(ui->dateMaxSelector->dateTime());

    result.first = minDate;
    result.second = maxDate;

    return result;
}

baseWorker::iteratorMode conditionWidget::typeMode() const
{
    if(folderCheckBox->isChecked())
        return fW::folderOnly;
    else if(fileCheckBox->isChecked())
        return fW::filesOnly;
    else
        return fW::allEntries;
}

void conditionWidget::on_modeSelector_currentIndexChanged(int index)
{
    if(currentFileModeRule == rD::sizeMode)
        ui->sizeViewSelector->setCurrentIndex(index);
    if((currentFileModeRule == rD::dateCreatedMode ||
        currentFileModeRule == rD::dateModifiedMode))
        ui->dateView_2->setCurrentIndex(index);
}

void conditionWidget::on_folderSelector_toggled(bool checked)
{
    if(checked)
        ui->fileSelector->setChecked(false);
}

void conditionWidget::on_fileSelector_toggled(bool checked)
{
    if(checked)
        ui->folderSelector->setChecked(false);
}
