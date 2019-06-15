#ifndef ABSTRACTFRAMEIMPLEMENTABLE_H
#define ABSTRACTFRAMEIMPLEMENTABLE_H

#include <qwidget.h>
#include <QMouseEvent>


class AbstractFrameImplementable : public QWidget
{
    Q_OBJECT
public:
    AbstractFrameImplementable(QWidget *parent = nullptr);

    bool isResizeable(){return resizeable;}
    void setResizeable(bool resizeMode){resizeable = resizeMode;}

signals:
    void sizeChanged(QSize newSize);

protected:

    void closeEvent(QCloseEvent *event);
    virtual void resizeEvent(QSize newSize) = 0;

private:
    bool resizeable = true;
};

#endif // ABSTRACTFRAMEIMPLEMENTABLE_H
