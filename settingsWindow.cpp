#include "settingsWindow.h"
#include "ui_settingsWindow.h"

settingsWindow::settingsWindow(QWidget *frameForm,settingsController *sCon, ruleController *rCon):
    QWidget(frameForm),
    ui(new Ui::settingsWindow)
{
    ui->setupUi(this);

    closeOnBox = ui->closeOnExitBox_2;
    countTimerEnableBox = ui->countTimerActivateBox_2;
    countTimerInterval = ui->countTImerIntervalEdit_2;
    enableRules = ui->enableRules_2;
    mView = ui->mainWidget;
    rControl = rCon;
    rulesView = ui->ruleItemView_2;
    ruleParentHeaderData= QStringList{"Regel titel","Handling","Mappe sti"};
    ruleChildrenHeaderData = QStringList{"Betingelse","Betingelses metode","Værdi"};
    sControl = sCon;
    view = ui->listWidget_2;
    vScroll = new QScrollBar(Qt::Vertical);

    // setValues..

    closeOnBox->setChecked(sCon->closeOnQuit());
    countTimerEnableBox->setChecked(sCon->countTimerEnabled());
    enableRules->setChecked(sCon->isRulesEnabled());
    countTimerInterval->setText(sCon->countTimerInterval());


    // Setfocus..
    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    // Rulesview related..

    rulesView->addTopLevelItems(rControl->rulesTreeItems());

    // ... Header related..


    rulesView->setColumnCount(3);
    rulesView->setHeaderHidden(false);
    rulesView->setHeaderLabels(ruleParentHeaderData);


    // Insert Icons..

    connect(view,SIGNAL(activated(QModelIndex)),
            this,SLOT(viewClicked(QModelIndex)));

    WidgetForm *p = static_cast<WidgetForm*>(parentWidget());
    p->setWidget(this,"Generelle indstillinger");
}

settingsWindow::~settingsWindow()
{
    delete ui;
    delete this;
}

void settingsWindow::setIconList(QList<myIcon> list)
{
    for(myIcon icon : list)
        new QListWidgetItem(icon,icon.name(),view);
}

bool settingsWindow::rulesEnabled()
{
    return sControl->isRulesEnabled();
}

void settingsWindow::focusOutEvent(QFocusEvent *event)
{
    if(event->type() == QFocusEvent::FocusOut)
    {
        activateWindow();
    }
}

void settingsWindow::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::FocusOut || event->type() == event->FocusAboutToChange)
    {
        event->ignore();
    }
    else
    {
        event->accept();
    }
}

void settingsWindow::closeEvent(QCloseEvent *event)
{
    if(event->type() == QEvent::Close)
    {
        emit destroyed();
        event->accept();
    }
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


void settingsWindow::recieveRule(rule r)
{
    rControl->insertRule(r);
    updateRulesView();
}

void settingsWindow::recieveModifiedRule(rule r, int index)
{
    rControl->replaceRule(r,index);
    updateRulesView();
}

void settingsWindow::on_insertRule_2_clicked()
{
    addRuleDialog *ruleDialog = new addRuleDialog(sControl->Paths(),new WidgetForm());
    connect(ruleDialog,&addRuleDialog::sendRule,this,&settingsWindow::recieveRule);
    ruleDialog->show();
}

void settingsWindow::on_lukKnap_2_clicked()
{
    sControl->setCloseOnExit(closeOnBox->isChecked());
    sControl->setTimerEnabled(countTimerEnableBox->isChecked());
    sControl->setTimerInterval(countTimerInterval->text().toInt());
    sControl->setRulesEnabled(enableRules->isChecked());

    close();
}

void settingsWindow::updateRulesView()
{
    rulesView->clear();
    rulesView->addTopLevelItems(rControl->rulesTreeItems());
}

void settingsWindow::on_fortrydKnap_2_clicked()
{
    emit close();
    close();
}

void settingsWindow::on_deleteRule_2_clicked()
{
    QModelIndex cIndex = rulesView->currentIndex();
    rControl->removeRule(cIndex.row());
    updateRulesView();
}

void settingsWindow::on_exitButton_clicked()
{
    close();
}

void settingsWindow::on_moveDownButton_2_clicked()
{
    QList<rule>*allRules = rControl->pointerToRules();
    int rowIndex = rulesView->currentIndex().row();
    if(rowIndex> 0)
    {
        allRules->move(rowIndex,rowIndex-1);
        rowIndex--;
    }
    updateRulesView();
}

void settingsWindow::on_moveUpButton_2_clicked()
{
    QList<rule>*allRules = rControl->pointerToRules();
    int rowIndex = rulesView->currentIndex().row();
    if(rowIndex <(allRules->count()-1))
    {
        allRules->move(rowIndex,rowIndex +1);
        rowIndex++;
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

void settingsWindow::on_editRule_2_clicked()
{
    if(rulesView->topLevelItemCount() == 0)
        return;
    int cIndex = rulesView->currentIndex().row();

    rule r = rControl->rules.at(cIndex);
    QStringList folderPaths = sControl->Paths();

    editRuleDialog *editDialog = new editRuleDialog(r,cIndex,folderPaths);

    connect(editDialog,&editRuleDialog::sendModifiedRule,this,&settingsWindow::recieveModifiedRule);

    editDialog->show();
}


void settingsWindow::on_ruleItemView_2_itemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    if(item->childCount() > 0)
        rulesView->setHeaderLabels(ruleParentHeaderData);
    else
        rulesView->setHeaderLabels(ruleChildrenHeaderData);
}
