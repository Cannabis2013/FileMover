#ifndef ABSTRACTFRAMEIMPLEMENTABLE_H
#define ABSTRACTFRAMEIMPLEMENTABLE_H

#include <qwidget.h>
#include <QMouseEvent>


class FrameImplementable : public QWidget
{
    Q_OBJECT
public:
    FrameImplementable(QWidget *parent = nullptr);

    bool Resizeable(){return resizeable;}
    void setResizeable(bool resizeMode){resizeable = resizeMode;}


protected:

    void closeEvent(QCloseEvent *event);

private:
    bool resizeable = true;


};

#endif // ABSTRACTFRAMEIMPLEMENTABLE_H
