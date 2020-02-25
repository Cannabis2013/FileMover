#include "abstractframe.h"

AbstractFrame::AbstractFrame(QWidget *parent):
    QWidget(parent)
{
}

void AbstractFrame::setWidgetTitle(QString title)
{
    _widgetTitle = title;
}

void AbstractFrame::closeEvent(QCloseEvent *event)
{
    if(event->type() == QEvent::Close)
    {
        emit destroyed();
        event->accept();
    }
}
