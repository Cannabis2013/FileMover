#include "settingsWindow.h"
#include "ui_settingsWindow.h"

settingsWindow::settingsWindow(QWidget *parent):
    QWidget(parent),
    ui(new Ui::settingsWindow)
{
    ui->setupUi(this);

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


    // Setfocus..

    // Rulesview related..

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

bool settingsWindow::rulesEnabled()
{
}

void settingsWindow::focusOutEvent(QFocusEvent *event)
{

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


void settingsWindow::recieveRule(Rule r)
{
}

void settingsWindow::recieveModifiedRule(Rule r, int index)
{
}

void settingsWindow::on_insertRule_2_clicked()
{

}

void settingsWindow::on_lukKnap_2_clicked()
{
    close();
}

void settingsWindow::updateRulesView()
{
}

void settingsWindow::on_fortrydKnap_2_clicked()
{
    emit close();
    close();
}

void settingsWindow::on_deleteRule_2_clicked()
{
}

void settingsWindow::on_exitButton_clicked()
{
    close();
}

void settingsWindow::on_moveDownButton_2_clicked()
{
}

void settingsWindow::on_moveUpButton_2_clicked()
{
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

}


void settingsWindow::on_ruleItemView_2_itemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    if(item->childCount() > 0)
        rulesView->setHeaderLabels(ruleParentHeaderData);
    else
        rulesView->setHeaderLabels(ruleChildrenHeaderData);
}
