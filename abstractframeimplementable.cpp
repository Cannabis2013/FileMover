#include "abstractframeimplementable.h"

AbstractFrameImplementable::AbstractFrameImplementable(QWidget *parent):
    MutuableWidget(parent)
{
}

void AbstractFrameImplementable::setWidgetTitle(QString title)
{
    widgetTitle = title;
}

void AbstractFrameImplementable::closeEvent(QCloseEvent *event)
{
    if(event->type() == QEvent::Close)
    {
        emit destroyed();
        event->accept();
    }
}
