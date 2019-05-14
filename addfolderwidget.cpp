#include "addfolderwidget.h"
#include "ui_addfolderwidget.h"

AddFolderWidget::AddFolderWidget() :
    FrameImplementable(),
    ui(new Ui::AddFolderWidget)
{
    ui->setupUi(this);
    fileWidget = ui->widget;
    pathWidget = ui->widget_2;
    vLayout = ui->verticalLayout;

    setWindowModality(Qt::ApplicationModal);

    setResizeable(false);
}

AddFolderWidget::~AddFolderWidget()
{
    close();
    delete ui;
}

void AddFolderWidget::sizeAdjust(QSizePolicy::Policy p, int H)
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

void AddFolderWidget::on_Add_Button_Clicked()
{
    QString filePath = pathWidget->text();
    emit sendPath(filePath);
    close();
}

