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
    ui->modeSelector->setCurrentText(rDefs.intervalConditionalList.at(0));

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
    ui->modeSelector->setCurrentText(rDefs.intervalConditionalList.at(1));

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
    ui->modeSelector->setCurrentText(rDefs.intervalConditionalList.at(0));

    ui->dateFixedSelector->setDate(dateTime.date());
    dateOperatorSelector->setCurrentText(rDefs.compareToString(cM));
}

void conditionWidget::setIntervalDate(QPair<myDateTime, myDateTime> iDate)
{
    rD rDefs;
    ui->modeSelector->setCurrentText(rDefs.intervalConditionalList.at(1));

    ui->dateMinSelector->setDate(iDate.first.date());
    ui->dateMaxSelector->setDate(iDate.second.date());
}

void conditionWidget::setTypeValues(Worker::iteratorMode tMode)
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
    rD rDefs;
    rD::fileFieldCondition conMode = rDefs.fieldConditionFromString(txt);
    currentFileModeRule = conMode;
    mainModeSelector->show();

    if(conMode == rD::filepathMode|| conMode == rD::extensionMode)
        mainModeView->setCurrentIndex(0);
    else if(conMode == rD::sizeMode)
        mainModeView->setCurrentIndex(1);
    else if(conMode == rD::dateCreatedMode ||
            conMode == rD::dateModifiedMode)
        mainModeView->setCurrentIndex(2);
    else if(conMode == rD::typeMode)
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
    rD rDefs;
    if(currentFileModeRule == rD::filepathMode || currentFileModeRule == rD::extensionMode)
    {
        mainModeSelector->setCurrentText(rDefs.compareToString(compare));
    }
    else if(currentFileModeRule == rD::sizeMode && currentCompareMode() != rD::interval)
    {
        fixedSizeModeSelector->setCurrentText(rDefs.compareToString(compare));
    }
    else if((currentFileModeRule == rD::dateCreatedMode ||
             currentFileModeRule == rD::dateModifiedMode) &&
            currentCompareMode() != rD::interval)
    {
        dateOperatorSelector->setCurrentText(rDefs.compareToString(compare));
    }
}

void conditionWidget::initDefaultOperators()
{
    fixedSizeModeSelector->clear();
    dateOperatorSelector->clear();

    rD rDefs;

    fixedSizeModeSelector->addItems(rDefs.compareOperatorsToStringList(rD::sizeMode));
    dateOperatorSelector->addItems(rDefs.compareOperatorsToStringList(rD::dateCreatedMode));
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
    rD rDefs;
    mainModeSelector->clear();
    currentFileModeRule = rDefs.fieldConditionFromString(condition);

    setCurrentView(condition);

    if(currentFileModeRule == rD::filepathMode || currentFileModeRule == rD::extensionMode)
    {
        mainModeSelector->addItems(rDefs.compareOperatorsToStringList(rD::filepathMode));
    }
    else if(currentFileModeRule == rD::sizeMode)
    {
        mainModeSelector->addItems(rDefs.intervalConditionalList);
        //fixedSizeModeSelector->clear();
        //fixedSizeModeSelector->addItems(rDefs.operatorsToStringList(rD::sizeMode));
    }
    else if(currentFileModeRule == rD::dateCreatedMode ||
            currentFileModeRule == rD::dateModifiedMode)
    {
        mainModeSelector->addItems(rDefs.intervalConditionalList);
    }
    else if(currentFileModeRule == rD::typeMode)
        mainModeSelector->addItem(condition);
}

ruleDefinitions::compareMode conditionWidget::currentCompareMode()
{
    rD rDefs;
    QString modeText = mainModeSelector->currentText();

    if(currentFileModeRule == rD::filepathMode || currentFileModeRule == rD::extensionMode)
        return rDefs.compareFromString(modeText);
    else if(currentFileModeRule == rD::sizeMode && modeText == rDefs.intervalConditionalList.at(0))
        return rDefs.compareFromString(ui->fixedSizeOperatorSelector->currentText());
    else if(currentFileModeRule == rD::sizeMode && modeText == rDefs.intervalConditionalList.at(1))
        return rD::interval;
    else if((currentFileModeRule == rD::dateCreatedMode ||
             currentFileModeRule == rD::dateModifiedMode) &&
            modeText == rDefs.intervalConditionalList.at(0))
        return rDefs.compareFromString(dateOperatorSelector->currentText());
    else if((currentFileModeRule == rD::dateCreatedMode ||
             currentFileModeRule == rD::dateModifiedMode) &&
            modeText == rDefs.intervalConditionalList.at(1))
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
    rD rDefs;
    QPair<ruleDefinitions::compareMode, myDateTime> result;
    QString dateOperator = dateOperatorSelector->currentText();
    rD::compareMode compareOperator = rDefs.compareFromString(dateOperator);

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

Worker::iteratorMode conditionWidget::typeMode() const
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
