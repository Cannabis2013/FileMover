#include "settingsWindow.h"
#include "ui_settingsWindow.h"

settingsWindow::settingsWindow(AbstractCoreApplication *coreApplication, QWidget *parent):
    FrameImplementable(parent),
    ui(new Ui::settingsWindow)
{
    ui->setupUi(this);

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

    // ... Header related..

    rulesView->setColumnCount(3);
    rulesView->setHeaderHidden(false);
    rulesView->setHeaderLabels(ruleParentHeaderData);

    // Insert Icons..

    connect(view,SIGNAL(activated(QModelIndex)),
            this,SLOT(viewClicked(QModelIndex)));

}

settingsWindow::~settingsWindow()
{
    delete ui;
    delete this;
}

void settingsWindow::setIconList(QList<MyIcon> list)
{
    for(MyIcon icon : list)
        new QListWidgetItem(icon,icon.name(),view);
}


void settingsWindow::closeEvent(QCloseEvent *event)
{
    if(event->type() == QEvent::Close)
    {
        emit destroyed();
        event->accept();
    }
}

void settingsWindow::mouseMoveEvent(QMouseEvent *event)
{
    QCursor cursor = QCursor(Qt::ArrowCursor);
    setCursor(cursor);
}


void settingsWindow::updateViews()
{
    updateRulesView();
}

void settingsWindow::viewClicked(QModelIndex i)
{
    if(i.isValid())
    {
        QListWidgetItem *item = view->item(i.row());
        QIcon ico = item->icon();
        emit iconSelected(ico);
        item = nullptr;
    }
}

void settingsWindow::closeBoxClicked(bool c)
{
    emit sendCheckBox(c);
}

void settingsWindow::on_insertRule_2_clicked()
{
    AddRuleDialog *ruleDialog = new AddRuleDialog(coreApplication->watchFolders());
    connect(ruleDialog,&AddRuleDialog::sendRule,coreApplication,&AbstractCoreApplication::insertRule);
    ruleDialog->show();
}

void settingsWindow::on_editRule_2_clicked()
{
    QString title = rulesView->currentItem()->text(0);
    Rule r = coreApplication->rule(title);

    EditRuleDialog *ruleDialog = new EditRuleDialog(r,coreApplication->watchFolders());
    connect(ruleDialog,&AbstractRuleDialog::replaceRule,coreApplication,&AbstractCoreApplication::replaceRule);

    ruleDialog->show();
}

void settingsWindow::on_lukKnap_2_clicked()
{
    close();
}

void settingsWindow::updateRulesView()
{
    rulesView->clear();
    QList<QTreeWidgetItem*> allRules = coreApplication->ruleItems();
    rulesView->addTopLevelItems(allRules);
}

void settingsWindow::on_fortrydKnap_2_clicked()
{
    close();
}

void settingsWindow::on_deleteRule_2_clicked()
{
    QString ruleTitle = rulesView->currentItem()->text(0);
    coreApplication->removeRule(ruleTitle);
}

void settingsWindow::on_exitButton_clicked()
{
    close();
}

void settingsWindow::on_moveDownButton_2_clicked()
{
    QTreeWidgetItem *cItem = rulesView->currentItem();

    int i = rulesView->indexOfTopLevelItem(cItem);
    if(i == -1)
        return;

    try {
        coreApplication->swapRule(i,i - 1);
    }
    catch (std::overflow_error e)
    {
        return;
    }

    updateRulesView();
}

void settingsWindow::on_moveUpButton_2_clicked()
{
    QTreeWidgetItem *cItem = rulesView->currentItem();

    int i = rulesView->indexOfTopLevelItem(cItem);
    if(i == -1)
        return;

    try {
        coreApplication->swapRule(i,i + 1);
    }
    catch (std::overflow_error e)
    {
        return;
    }

    updateRulesView();
}

void settingsWindow::on_countTImerIntervalEdit_2_returnPressed()
{
    int min = ui->countTImerIntervalEdit_2->text().toInt();
    emit sendInterval(min);
}

void settingsWindow::on_countTimerActivateBox_2_toggled(bool checked)
{
    emit enableTimer(checked);
}

void settingsWindow::on_ruleItemView_2_itemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    if(item->childCount() > 0)
        rulesView->setHeaderLabels(ruleParentHeaderData);
    else
        rulesView->setHeaderLabels(ruleChildrenHeaderData);
}
