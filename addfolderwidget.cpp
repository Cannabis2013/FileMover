#include "addfolderwidget.h"
#include "ui_addfolderwidget.h"

AddFolderWidget::AddFolderWidget() :
    AbstractFrame(),
    ui(new Ui::AddFolderWidget)
{
    ui->setupUi(this);
    fileWidget = ui->widget;
    pathWidget = ui->widget_2;
    vLayout = ui->verticalLayout;

    setResizeable(false);
}

AddFolderWidget::~AddFolderWidget()
{
    close();
    delete ui;
}

void AddFolderWidget::resizeNotify(QSize newSize)
{
    emit sizeChanged(newSize);
}

void AddFolderWidget::sizeAdjust(QSizePolicy::Policy p, int H)
{
    QSizePolicy::Policy hPol = sizePolicy().horizontalPolicy();
    QRect maxHeight = QApplication::desktop()->screenGeometry(this);
    setSizePolicy(hPol,p);
    if(p == QSizePolicy::Fixed)
    {
        lastSavedHeight = height();
        setFixedHeight(height() - H);
    }
    else
    {
        setFixedHeight(lastSavedHeight);
        setMaximumHeight(maxHeight.height());
    }
    resizeNotify(size());
}

void AddFolderWidget::on_Add_Button_Clicked()
{
    QString filePath = pathWidget->text();
    emit sendPath(filePath);
    close();
}

