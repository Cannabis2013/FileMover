#include "topframeform.h"
#include "ui_topframeform.h"

TopFrameForm::TopFrameForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TopFrameForm)
{
    ui->setupUi(this);
    topWidget = ui->topWidget;
    exitButton = ui->exitButton;

    frameTitle = ui->frameTitle;
}

TopFrameForm::~TopFrameForm()
{
    delete ui;
}

void TopFrameForm::mouseMoveEvent(QMouseEvent *event)
{
    int buttonLeftBound = width() - exitButton->width();
    if(event->pos().x() >= buttonLeftBound)
        topWidget->setCursor(QCursor(Qt::ArrowCursor));
    else
        topWidget->setCursor(QCursor(Qt::SizeAllCursor));

    if(isMousePressed)
        emit moveParent(event->pos() - mOffset);
}

void TopFrameForm::mouseReleaseEvent(QMouseEvent *event)
{
    isMousePressed = false;
}

void TopFrameForm::mousePressEvent(QMouseEvent *event)
{
    int buttonLeftBound = width() - exitButton->width();
    if(event->pos().x() >= buttonLeftBound)
        event->ignore();

    isMousePressed = true;
    mOffset = event->pos();
}

QString TopFrameForm::FrameTitle() const
{
    return frameTitle->text();
}

void TopFrameForm::setFrameTitle(QString title)
{
    frameTitle->setText(title);
}
