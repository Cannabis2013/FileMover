#include "topframeform.h"
#include "ui_topframeform.h"

TopFrameForm::TopFrameForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TopFrameForm)
{
    topWidget = ui->topWidget;
    ui->setupUi(this);

    setMouseTracking(true);
    frameTitle = ui->frameTitle;
}

TopFrameForm::~TopFrameForm()
{
    delete ui;
}

void TopFrameForm::mouseMoveEvent(QMouseEvent *event)
{
    //QCursor Cursor;
    //Cursor.setShape(Qt::SizeAllCursor);
    //setCursor(Cursor);
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
