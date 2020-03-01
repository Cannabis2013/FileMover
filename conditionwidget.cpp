#include "conditionwidget.h"
#include "ui_conditionwidget.h"

conditionWidget::conditionWidget(QWidget *parent) :
    QWidget(parent),
    GenericServiceInjector(new RuleDefinitions()),
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

void conditionWidget::setConditionalFixedSize(QPair<int, QString> fSize, RRT::RuleCompareCriteria cMode)
{
    RuleDefinitions rDefs;
    ui->modeSelector->setCurrentText(rDefs.intervalConditionalList.at(0));

    ui->fixedSizeSelector->setValue(fSize.first);
    ui->fixedSizeUnitSelector->setCurrentText(fSize.second);
    ui->fixedSizeOperatorSelector->setCurrentText(rDefs.buildStringFromCompareCriteria(cMode));
}

void conditionWidget::setConditionalIntervalSize(QPair<QPair<int, QString>, QPair<int, QString> > iSize)
{
    RuleDefinitions rDefs;
    ui->modeSelector->setCurrentText(rDefs.intervalConditionalList.at(1));

    QPair<int,QString> minSize = iSize.first;
    QPair<int,QString> maxSize = iSize.second;

    ui->sizeIntervalMinSelector->setValue(minSize.first);
    ui->sizeIntervalMinUnit->setCurrentText(minSize.second);

    ui->sizeIntervalMaxSelector->setValue(maxSize.first);
    ui->sizeIntervalMaxUnit->setCurrentText(maxSize.second);
}

void conditionWidget::setFixedDate(const myDateTime &dateTime)
{
    RuleDefinitions rDefs;
    ui->modeSelector->setCurrentText(rDefs.intervalConditionalList.at(0));
    ui->dateFixedSelector->setDate(dateTime.date());
}

void conditionWidget::setIntervalDate(QPair<myDateTime, myDateTime> iDate)
{
    RuleDefinitions rDefs;
    ui->modeSelector->setCurrentText(rDefs.intervalConditionalList.at(1));

    ui->dateMinSelector->setDate(iDate.first.date());
    ui->dateMaxSelector->setDate(iDate.second.date());
}

void conditionWidget::setTypeValues(fW::iteratorMode tMode)
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
    /*
     * Sets the view according to the current selected file condition
     */

    RuleDefinitions rDefs;
    RRT::RuleCriteria conMode = rDefs.buildCriteriaFromString(txt);
    currentFileModeRule = conMode;
    mainModeSelector->show();

    if(conMode == RRT::nonConditionalMode)
    {
        mainModeSelector->hide();
        mainModeView->setCurrentIndex(4);
    }
    else if(conMode == RRT::fileSize)
        mainModeView->setCurrentIndex(1);
    else if(conMode == RRT::fileCreatedMode ||
            conMode == RRT::fileModifiedMode)
        mainModeView->setCurrentIndex(2);
    else
        mainModeView->setCurrentIndex(0);
}

void conditionWidget::setCurrentView(int index)
{
    mainModeView->setCurrentIndex(index);
}

void conditionWidget::setCompareView(RRT::RuleCompareCriteria compare)
{
    RuleDefinitions rDefs;

    if(currentFileModeRule == RRT::fileSize && currentCompareMode() != RRT::interval)
    {
        fixedSizeModeSelector->setCurrentText(rDefs.buildStringFromCompareCriteria(compare));
    }
    else if((currentFileModeRule == RRT::fileCreatedMode ||
             currentFileModeRule == RRT::fileModifiedMode) &&
            currentCompareMode() != RRT::interval)
    {
        dateOperatorSelector->setCurrentText(rDefs.buildStringFromCompareCriteria(compare));
    }
    else
    {
        mainModeSelector->setCurrentText(rDefs.buildStringFromCompareCriteria(compare));
    }
}

void conditionWidget::initDefaultOperators()
{
    fixedSizeModeSelector->clear();
    dateOperatorSelector->clear();

    RuleDefinitions rDefs;

    fixedSizeModeSelector->addItems(rDefs.fileCompareModesToStringList(RRT::fileSize));
    dateOperatorSelector->addItems(rDefs.fileCompareModesToStringList(RRT::fileCreatedMode));
}

void conditionWidget::initDefaultUnits()
{
    fixedSizeUnitSelector->clear();
    sizeMinIntervalUnitSelector->clear();
    sizeMaxIntervalUnitSelector->clear();

    RuleDefinitions rDefs;
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
    RuleDefinitions rDefs;
    mainModeSelector->clear();
    currentFileModeRule = rDefs.buildCriteriaFromString(condition);

    setCurrentView(condition);

    if(currentFileModeRule == RRT::nonConditionalMode)
    {
        return;
    }
    else if(currentFileModeRule == RRT::fileSize)
    {
        mainModeSelector->addItems(rDefs.intervalConditionalList);
        //fixedSizeModeSelector->clear();
        //fixedSizeModeSelector->addItems(rDefs.operatorsToStringList(RuleRelatedTypes::sizeMode));
    }
    else if(currentFileModeRule == RRT::fileCreatedMode ||
            currentFileModeRule == RRT::fileModifiedMode)
    {
        mainModeSelector->addItems(rDefs.intervalConditionalList);
    }
    else
        mainModeSelector->addItems(rDefs.fileCompareModesToStringList(RRT::filepathMode));
}

RRT::RuleCompareCriteria conditionWidget::currentCompareMode()
{
    RuleDefinitions rDefs;
    QString modeText = mainModeSelector->currentText();

    if(currentFileModeRule == RRT::fileBaseMode ||
            currentFileModeRule == RRT::filepathMode ||
            currentFileModeRule == RRT::fileExtensionMode)
        return rDefs.fileCompareEntityFromString(modeText);
    else if(currentFileModeRule == RRT::fileSize && modeText == rDefs.intervalConditionalList.at(0))
        return rDefs.fileCompareEntityFromString(ui->fixedSizeOperatorSelector->currentText());
    else if(currentFileModeRule == RRT::fileSize && modeText == rDefs.intervalConditionalList.at(1))
        return RRT::interval;
    else if((currentFileModeRule == RRT::fileCreatedMode ||
             currentFileModeRule == RRT::fileModifiedMode) &&
            modeText == rDefs.intervalConditionalList.at(0))
        return rDefs.fileCompareEntityFromString(dateOperatorSelector->currentText());
    else if((currentFileModeRule == RRT::fileCreatedMode ||
             currentFileModeRule == RRT::fileModifiedMode) &&
            modeText == rDefs.intervalConditionalList.at(1))
        return RRT::interval;
    else
        return RRT::noCompareModeSet;
}

QString conditionWidget::keyWordValues() const
{
    return ui->keyWordSelector->text();
}

QPair<quint64, QString> conditionWidget::fixedSizeValues() const
{
    QPair<quint64,QString> result;
    uint val = fixedSizeSelector->value();
    QString unit = fixedSizeUnitSelector->currentText();

    result.first = val;
    result.second = unit;

    return result;
}

QPair<QPair<quint64, QString>, QPair<quint64, QString> > conditionWidget::intervalSizeValues() const
{
    QPair<QPair<quint64,QString>,QPair<quint64,QString>> result;

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

myDateTime conditionWidget::fixedConditionalDate() const
{
    QDateTime dt = ui->dateFixedSelector->dateTime();
    return myDateTime(dt);
}

QPair<myDateTime, myDateTime> conditionWidget::intervalDates() const
{
    QPair<myDateTime, myDateTime>result;
    myDateTime minDate(ui->dateMinSelector->dateTime()), maxDate(ui->dateMaxSelector->dateTime());

    result.first = minDate;
    result.second = maxDate;

    return result;
}

fW::iteratorMode conditionWidget::typeMode() const
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
    if(currentFileModeRule == RRT::fileSize)
        ui->sizeViewSelector->setCurrentIndex(index);
    if((currentFileModeRule == RRT::fileCreatedMode ||
        currentFileModeRule == RRT::fileModifiedMode))
        ui->stackedDateView->setCurrentIndex(index);
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
