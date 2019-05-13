#include "customsplitterhandle.h"

CustomSplitterHandle::CustomSplitterHandle(Qt::Orientation direction, QSplitter *parent, QString label):
    QSplitterHandle(direction,parent),handleLabel(label)
{
}

void CustomSplitterHandle::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect eRect = event->rect();
    int ew = eRect.width(),
            eh = eRect.height(),
            handleHeight = 32;
    QSplitter *mys = static_cast<QSplitter*>(parent());
    mys->setHandleWidth(handleHeight);
    QPen rectPen(Qt::SolidPattern),borderPen(Qt::SolidPattern);
    rectPen.setColor(QColor(132,66,15));
    borderPen.setColor(QColor(Qt::black));
    borderPen.setWidth(2);
    painter.setFont(QFont("Times",20));
    if(orientation() == Qt::Vertical)
    {

        painter.fillRect(0,0,ew,handleHeight,QColor(189,89,32));

        // Draw borders
        painter.setPen(borderPen);
        painter.drawLine(0,handleHeight-1,ew,handleHeight-1);

        painter.setPen(rectPen);
        painter.drawText(event->rect(),Qt::AlignCenter,handleLabel);
    }
    else
    {
        painter.fillRect(0,0,handleHeight,eh,QColor(189,89,32));
    }
}
