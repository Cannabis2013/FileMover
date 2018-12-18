#include "addfolderwidget.h"
#include "ui_addfolderwidget.h"

addFolderWidget::addFolderWidget() :
    ui(new Ui::addFolderWidget)
{
    ui->setupUi(this);
    fileWidget = ui->widget;
    pathWidget = ui->widget_2;
    vLayout = ui->verticalLayout;

}

addFolderWidget::~addFolderWidget()
{
    delete ui;
}

void addFolderWidget::sizeAdjust(QSizePolicy::Policy p, int H)
{
    QSizePolicy::Policy hPol = sizePolicy().horizontalPolicy();
    QRect maxHeight = QApplication::desktop()->screenGeometry(this);
    setSizePolicy(hPol,p);
    if(p == QSizePolicy::Fixed)
    {
        tempHeight = height();
        setFixedHeight(this->height() - H);
    }
    else
    {
        setFixedHeight(tempHeight);
        setMaximumHeight(maxHeight.height());
    }
}

void addFolderWidget::on_Add_Button_Clicked()
{
    QString filePath = pathWidget->text();
    emit sendPath(filePath);
    close();
}

