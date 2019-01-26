#include "topframeform.h"
#include "ui_topframeform.h"

TopFrameForm::TopFrameForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TopFrameForm)
{
    ui->setupUi(this);

    frameTitle = ui->frameTitle;
}

TopFrameForm::~TopFrameForm()
{
    delete ui;
}

void TopFrameForm::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        emit moveParent(event->pos() - mOffset);
    }
}

void TopFrameForm::mousePressEvent(QMouseEvent *event)
{
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
