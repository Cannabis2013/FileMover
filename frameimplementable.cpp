#include "frameimplementable.h"

FrameImplementable::FrameImplementable(QWidget *parent):
    QWidget (parent)
{

}

void FrameImplementable::closeEvent(QCloseEvent *event)
{
    if(event->type() == QEvent::Close)
    {
        emit destroyed();
        event->accept();
    }
}
