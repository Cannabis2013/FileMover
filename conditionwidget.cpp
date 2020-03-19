#include "conditionwidget.h"
#include "ui_conditionwidget.h"

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

    mainModeView = ui->modeWidget;

    mainModeView->setCurrentIndex(4);
}

conditionWidget::~conditionWidget()
{
    delete ui;
}

void conditionWidget::setRulesDefinitionsService(IRuleDefinitions *service)
{
    RuleServicesInjector::setRulesDefinitionsService(service);
    initDefaultOperators();
    initDefaultUnits();

}

void conditionWidget::setKeyWords(QString kW)
{
    ui->keyWordSelector->setText(kW);
}

void conditionWidget::setConditionalFixedSize(QPair<int, QString> fSize, const int &cMode)
{
    ui->modeSelector->setCurrentText(ruleDefinitionsService()->intervalConditionalList().at(0));

    ui->fixedSizeSelector->setValue(fSize.first);
    ui->fixedSizeUnitSelector->setCurrentText(fSize.second);
    ui->fixedSizeOperatorSelector->setCurrentText(ruleDefinitionsService()->buildStringFromCompareCriteria(cMode));
}

void conditionWidget::setConditionalIntervalSize(QPair<QPair<int, QString>, QPair<int, QString> > iSize)
{
    ui->modeSelector->setCurrentText(ruleDefinitionsService()->intervalConditionalList().at(1));

    QPair<int,QString> minSize = iSize.first;
    QPair<int,QString> maxSize = iSize.second;

    ui->sizeIntervalMinSelector->setValue(minSize.first);
    ui->sizeIntervalMinUnit->setCurrentText(minSize.second);

    ui->sizeIntervalMaxSelector->setValue(maxSize.first);
    ui->sizeIntervalMaxUnit->setCurrentText(maxSize.second);
}

void conditionWidget::setFixedDate(const QDateTime &dateTime)
{
    ui->modeSelector->setCurrentText(ruleDefinitionsService()->intervalConditionalList().at(0));
    ui->dateFixedSelector->setDate(dateTime.date());
}

void conditionWidget::setIntervalDate(QPair<QDateTime, QDateTime> iDate)
{
    ui->modeSelector->setCurrentText(ruleDefinitionsService()->intervalConditionalList().at(1));

    ui->dateMinSelector->setDate(iDate.first.date());
    ui->dateMaxSelector->setDate(iDate.second.date());
}

void conditionWidget::setTypeValues(const int &tMode)
{
    if(tMode == folderOnly)
    {
        folderCheckBox->setChecked(true);
        fileCheckBox->setChecked(false);
    }
    else if(tMode == filesOnly)
    {
        fileCheckBox->setChecked(true);
        folderCheckBox->setChecked(false);
    }
    else if(tMode == allEntries)
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

    auto conMode = ruleDefinitionsService()->buildCriteriaFromString(txt);
    currentFileModeRule = conMode;
    mainModeSelector->show();

    if(conMode == DefaultRulesContext::NonConditionalMode)
    {
        mainModeSelector->hide();
        mainModeView->setCurrentIndex(4);
    }
    else if(conMode == DefaultRulesContext::FileSizeMode)
        mainModeView->setCurrentIndex(1);
    else if(conMode == DefaultRulesContext::FileCreatedMode ||
            conMode == DefaultRulesContext::FileModifiedMode)
        mainModeView->setCurrentIndex(2);
    else
        mainModeView->setCurrentIndex(0);
}

void conditionWidget::setCurrentView(int index)
{
    mainModeView->setCurrentIndex(index);
}

void conditionWidget::setCompareView(const int &compare)
{
    if(currentFileModeRule == DefaultRulesContext::FileSizeMode && currentCompareMode() != DefaultRulesContext::Interval)
    {
        fixedSizeModeSelector->setCurrentText(ruleDefinitionsService()->buildStringFromCompareCriteria(compare));
    }
    else if((currentFileModeRule == DefaultRulesContext::FileCreatedMode ||
             currentFileModeRule == DefaultRulesContext::FileModifiedMode) &&
            currentCompareMode() != DefaultRulesContext::Interval)
    {
        dateOperatorSelector->setCurrentText(ruleDefinitionsService()->buildStringFromCompareCriteria(compare));
    }
    else
    {
        mainModeSelector->setCurrentText(ruleDefinitionsService()->buildStringFromCompareCriteria(compare));
    }
}

void conditionWidget::initDefaultOperators()
{
    fixedSizeModeSelector->clear();
    dateOperatorSelector->clear();


    fixedSizeModeSelector->addItems(ruleDefinitionsService()->fileCompareModesToStringList(DefaultRulesContext::FileSizeMode));
    dateOperatorSelector->addItems(ruleDefinitionsService()->fileCompareModesToStringList(DefaultRulesContext::FileCreatedMode));
}

void conditionWidget::initDefaultUnits()
{
    fixedSizeUnitSelector->clear();
    sizeMinIntervalUnitSelector->clear();
    sizeMaxIntervalUnitSelector->clear();

    if(ruleDefinitionsService() == nullptr)
        return;

    fixedSizeUnitSelector->addItems(ruleDefinitionsService()->sizeUnits());
    sizeMinIntervalUnitSelector->addItems(ruleDefinitionsService()->sizeUnits());
    sizeMaxIntervalUnitSelector->addItems(ruleDefinitionsService()->sizeUnits());
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
    currentFileModeRule = ruleDefinitionsService()->buildCriteriaFromString(condition);

    setCurrentView(condition);

    if(currentFileModeRule == DefaultRulesContext::NonConditionalMode)
    {
        return;
    }
    else if(currentFileModeRule == DefaultRulesContext::FileSizeMode)
    {
        mainModeSelector->addItems(ruleDefinitionsService()->intervalConditionalList());
        fixedSizeModeSelector->clear();
        fixedSizeModeSelector->addItem(ruleDefinitionsService()->buildStringFromCriteria(DefaultRulesContext::FileSizeMode));
    }
    else if(currentFileModeRule == DefaultRulesContext::FileCreatedMode ||
            currentFileModeRule == DefaultRulesContext::FileModifiedMode)
    {
        mainModeSelector->addItems(ruleDefinitionsService()->intervalConditionalList());
    }
    else
        mainModeSelector->addItems(ruleDefinitionsService()->fileCompareModesToStringList(DefaultRulesContext::FileNameMode));
}

int conditionWidget::currentCompareMode()
{
    QString modeText = mainModeSelector->currentText();

    if(currentFileModeRule == DefaultRulesContext::FileBaseMode ||
            currentFileModeRule == DefaultRulesContext::FileNameMode ||
            currentFileModeRule == DefaultRulesContext::FileExtensionMode)
        return ruleDefinitionsService()->fileCompareEntityFromString(modeText);
    else if(currentFileModeRule == DefaultRulesContext::FileSizeMode && modeText == ruleDefinitionsService()->intervalConditionalList().at(0))
        return ruleDefinitionsService()->fileCompareEntityFromString(ui->fixedSizeOperatorSelector->currentText());
    else if(currentFileModeRule == DefaultRulesContext::FileSizeMode && modeText == ruleDefinitionsService()->intervalConditionalList().at(1))
        return DefaultRulesContext::Interval;
    else if((currentFileModeRule == DefaultRulesContext::FileCreatedMode ||
             currentFileModeRule == DefaultRulesContext::FileModifiedMode) &&
            modeText == ruleDefinitionsService()->intervalConditionalList().at(0))
        return ruleDefinitionsService()->fileCompareEntityFromString(dateOperatorSelector->currentText());
    else if((currentFileModeRule == DefaultRulesContext::FileCreatedMode ||
             currentFileModeRule == DefaultRulesContext::FileModifiedMode) &&
            modeText == ruleDefinitionsService()->intervalConditionalList().at(1))
        return DefaultRulesContext::Interval;
    else
        return DefaultRulesContext::NoCompareModeSet;
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

QDateTime conditionWidget::fixedConditionalDate() const
{
    QDateTime dt = ui->dateFixedSelector->dateTime();
    return dt;
}

QPair<QDateTime, QDateTime> conditionWidget::intervalDates() const
{
    QPair<QDateTime, QDateTime>result;
    QDateTime minDate(ui->dateMinSelector->dateTime()), maxDate(ui->dateMaxSelector->dateTime());

    result.first = minDate;
    result.second = maxDate;

    return result;
}

int conditionWidget::typeMode() const
{
    if(folderCheckBox->isChecked())
        return folderOnly;
    else if(fileCheckBox->isChecked())
        return filesOnly;
    else
        return allEntries;
}

void conditionWidget::on_modeSelector_currentIndexChanged(int index)
{
    if(currentFileModeRule == DefaultRulesContext::FileSizeMode)
        ui->sizeViewSelector->setCurrentIndex(index);
    if((currentFileModeRule == DefaultRulesContext::FileCreatedMode ||
        currentFileModeRule == DefaultRulesContext::FileModifiedMode))
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
