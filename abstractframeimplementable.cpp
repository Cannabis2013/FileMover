#include "abstractframeimplementable.h"

AbstractFrameImplementable::AbstractFrameImplementable(QWidget *parent):
    QWidget (parent)
{

}

void AbstractFrameImplementable::closeEvent(QCloseEvent *event)
{
    if(event->type() == QEvent::Close)
    {
        emit destroyed();
        event->accept();
    }
}
