#ifndef ABSTRACTFRAMEIMPLEMENTABLE_H
#define ABSTRACTFRAMEIMPLEMENTABLE_H

#include <qwidget.h>
#include <QMouseEvent>


class AbstractFrameImplementable : public QWidget
{
    Q_OBJECT
public:
    AbstractFrameImplementable(QWidget *parent);

signals:
    void sendPosition(QPoint pos);

protected:


};

#endif // ABSTRACTFRAMEIMPLEMENTABLE_H
