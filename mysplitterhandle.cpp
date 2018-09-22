#include "mysplitterhandle.h"

mySplitterHandle::mySplitterHandle(Qt::Orientation direction, QSplitter *parent, QString label):
    QSplitterHandle(direction,parent),handleLabel(label)
{
}

void mySplitterHandle::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect eRect = event->rect();
    int ew = eRect.width(),eh = eRect.height(),hh = 32;
    QSplitter *mys = (QSplitter*) parent();
    mys->setHandleWidth(hh);
    QPen rectPen(Qt::SolidPattern),borderPen(Qt::SolidPattern);
    rectPen.setColor(QColor(132,66,15));
    borderPen.setColor(QColor(Qt::black));
    borderPen.setWidth(2);
    painter.setFont(QFont("Times",20));
    if(orientation() == Qt::Vertical)
    {
        painter.fillRect(0,0,ew,hh,QColor(189,89,32));

        // Draw borders
        painter.setPen(borderPen);
        painter.drawLine(0,hh-1,ew,hh-1);

        painter.setPen(rectPen);
        painter.drawText(event->rect(),Qt::AlignCenter,handleLabel);
    }
    else
    {
        painter.fillRect(0,0,hh,eh,QColor(189,89,32));
    }
}
