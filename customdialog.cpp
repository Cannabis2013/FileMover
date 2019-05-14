#include "customdialog.h"


CustomDialog::CustomDialog(AbstractFrameImplementable *implementable, bool applicationModal, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomDialog)
{
    ui->setupUi(this);

    widgetFrame = ui->frame;
    mainWidget = nullptr;
    topFrame = ui->TopFrame;
    grid = ui->widgetLayout;
    setWindowFlag(Qt::FramelessWindowHint);

    if(applicationModal)
        setWindowModality(Qt::ApplicationModal);

    connect(topFrame,&TopFrameForm::moveParent,this,&CustomDialog::moveGlobalEvent);
    setWidget(implementable);
    show();
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

CustomDialog::~CustomDialog()
{
    delete ui;
}

void CustomDialog::setWidget(AbstractFrameImplementable *implementable, QString title)
{

    mainWidget = implementable;

    mainWidget->setWindowFlag(Qt::FramelessWindowHint);

    if(grid->itemAtPosition(0,0) != nullptr)
    {
        QLayoutItem * item = grid->itemAtPosition(0,0);
        grid->removeItem(item);
    }

    connect(mainWidget,&QWidget::destroyed,this,&CustomDialog::close);
    grid->addWidget(mainWidget,0,0);

    setFrameTitle(title);
}

void CustomDialog::setFrameTitle(QString title)
{
    topFrame->setFrameTitle(title);
}

QString CustomDialog::FrameTitle()
{
    return topFrame->FrameTitle();
}

void CustomDialog::on_exitButton_clicked()
{
    close();
}

void CustomDialog::closeEvent(QCloseEvent *event)
{
    if(mainWidget != nullptr)
        mainWidget->close();
}

void CustomDialog::mouseMoveEvent(QMouseEvent *event)
{
    int borderWidth = 4;
    int rightBorder = width() - borderWidth,
            lowerBorder = height() - borderWidth;


    if(event->pos().x() >= rightBorder && event->pos().y() >= lowerBorder)
    {
        QCursor cursor = QCursor(Qt::SizeFDiagCursor);
        setCursor(cursor);
    }
    else {
        QCursor cursor = QCursor(Qt::ArrowCursor);
        setCursor(cursor);
    }
}



void CustomDialog::moveGlobalEvent(QPoint pos)
{
    move(mapToParent(pos));
}
