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

    void setWidgetTitle(QString title);

    QString getWidgetTitle() {return widgetTitle;}

signals:
    void sizeChanged(QSize newSize);

protected:

    void closeEvent(QCloseEvent *event);
    virtual void resizeEvent(QSize newSize) = 0;

private:
    bool resizeable = true;
    QString widgetTitle = "Frametitle";
};

#endif // ABSTRACTFRAMEIMPLEMENTABLE_H
