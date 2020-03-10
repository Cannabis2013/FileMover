#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(AbstractApplicationService *coreApplication, IDefinitions *ruleService, QWidget *parent):
    AbstractFrame(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    this->ruleService = ruleService;
    this->coreApplication = coreApplication;
    closeOnBox = ui->closeOnExitBox_2;
    countTimerEnableBox = ui->countTimerActivateBox_2;
    countTimerInterval = ui->countTImerIntervalEdit_2;
    enableRules = ui->enableRules_2;
    mView = ui->mainWidget;
    rulesView = ui->ruleItemView_2;
    ruleParentHeaderData= QStringList{"Regel titel","Handling","Mappe sti"};
    ruleChildrenHeaderData = QStringList{"Betingelse","Betingelses metode","Værdi"};
    view = ui->listWidget_2;
    vScroll = new QScrollBar(Qt::Vertical);

    rulesView->setIconSize(QSize(32,32));

    QFont stdFont = QApplication::font();
    stdFont.setPointSize(12);

    rulesView->setFont(stdFont);
    // ... Header related..

    rulesView->setColumnCount(3);
    rulesView->setHeaderHidden(false);
    rulesView->setHeaderLabels(ruleParentHeaderData);

    // Insert Icons..

    connect(view,SIGNAL(activated(QModelIndex)),
            this,SLOT(viewClicked(QModelIndex)));

    initializeState();
    updateView();
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
    delete this;
}

void SettingsWindow::setIconList(QList<Icon> list)
{
    for(Icon icon : list)
        new QListWidgetItem(icon,icon.name(),view);
}

void SettingsWindow::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    QCursor cursor = QCursor(Qt::ArrowCursor);
    setCursor(cursor);
}

void SettingsWindow::viewClicked(QModelIndex i)
{
    if(i.isValid())
    {
        QListWidgetItem *item = view->item(i.row());
        QIcon ico = item->icon();
        emit iconSelected(ico);
        item = nullptr;
    }
}

void SettingsWindow::closeBoxClicked(bool c)
{
    emit sendCheckBox(c);
}

void SettingsWindow::on_insertRule_2_clicked()
{
    QStringList watchFolders = coreApplication->watchFolders();
    AddRuleDialog *ruleDialog =  new AddRuleDialog(watchFolders,ruleService);
    CustomDialog *dialog = new CustomDialog(ruleDialog,true);
    connect(ruleDialog,&AddRuleDialog::sendRule,coreApplication,&AbstractApplicationService::insertRule);
    dialog->show();
}

void SettingsWindow::on_editRule_2_clicked()
{
    if(rulesView->currentItem() == nullptr)
        return;

    QString title = rulesView->currentItem()->text(0);
    Rule r = coreApplication->rule(title);

    EditRuleDialog *ruleDialog = new EditRuleDialog(r,coreApplication->watchFolders(),ruleService);
    CustomDialog *dialog = new CustomDialog(ruleDialog,true);
    connect(ruleDialog,&AbstractRuleDialog::replaceRule,coreApplication,&AbstractApplicationService::replaceRule);

    dialog->show();
}


void SettingsWindow::updateView()
{
    rulesView->clear();
    QList<QTreeWidgetItem*> allRules = coreApplication->ruleItemModels();
    rulesView->addTopLevelItems(allRules);
}

void SettingsWindow::on_deleteRule_2_clicked()
{
    if(rulesView->currentItem() == nullptr)
        return;

    QString ruleTitle = rulesView->currentItem()->text(0);
    coreApplication->removeRule(ruleTitle);
}

void SettingsWindow::on_exitButton_clicked()
{
    close();
}

void SettingsWindow::on_moveDownButton_2_clicked()
{
    QTreeWidgetItem *cItem = rulesView->currentItem();

    int i = rulesView->indexOfTopLevelItem(cItem);
    if(i == -1 || cItem == nullptr)
        return;

    try {
        coreApplication->swapRule(i,i - 1);
    }
    catch (const std::out_of_range *e)
    {
        return;
    }
}

void SettingsWindow::on_moveUpButton_2_clicked()
{
    QTreeWidgetItem *cItem = rulesView->currentItem();

    int i = rulesView->indexOfTopLevelItem(cItem);
    if(i == -1 || cItem == nullptr)
        return;

    try {
        coreApplication->swapRule(i,i + 1);
    }
    catch (const std::out_of_range *e)
    {
        return;
    }
}

void SettingsWindow::on_countTImerIntervalEdit_2_returnPressed()
{
    int min = ui->countTImerIntervalEdit_2->text().toInt();
    emit sendInterval(min);
}

void SettingsWindow::on_saveButton_clicked()
{
    // Get current settings
    auto currentSettings = coreApplication->settingsState();

    // Initialize variables
    auto closeOnExit = closeOnBox->isChecked();
    auto rulesEnabled = enableRules->isChecked();
    auto ruleCountInterval = countTimerInterval->text().toInt();

    //Build new settings based on current and variables
    auto settings = SettingsDelegateBuilder::buildSettingsDelegate(closeOnExit,
                                                                   currentSettings->ruleTimerEnabled(),
                                                                   rulesEnabled,ruleCountInterval,
                                                                   currentSettings->mainGuiGeometry());
    coreApplication->setSettings(settings);
    close();
}

void SettingsWindow::initializeState()
{
    auto settings = coreApplication->settingsState();
    closeOnBox->setChecked(settings->closeOnExit());
    enableRules->setChecked(settings->rulesEnabled());
    countTimerInterval->setText(QString::number(settings->ruleCountInterval()));
}

void SettingsWindow::on_countTimerActivateBox_2_toggled(bool checked)
{
    emit enableTimer(checked);
}

void SettingsWindow::on_ruleItemView_2_itemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    if(item->childCount() > 0)
        rulesView->setHeaderLabels(ruleParentHeaderData);
    else
        rulesView->setHeaderLabels(ruleChildrenHeaderData);
}
