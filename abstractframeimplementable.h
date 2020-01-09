#ifndef ABSTRACTFRAMEIMPLEMENTABLE_H
#define ABSTRACTFRAMEIMPLEMENTABLE_H

#include <qwidget.h>
#include <QMouseEvent>
#include "NotifyDomain.h"

class AbstractFrameImplementable :
        public QWidget,
        public IMutableObject
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
    void stateChanged();

protected:

    // Reimplemented
    void closeEvent(QCloseEvent *event);

    // Pure virtual
    virtual void resizeNotify(QSize newSize) = 0;

private:
    bool resizeable = true;
    QString widgetTitle = "Frametitle";

public:
};

#endif // ABSTRACTFRAMEIMPLEMENTABLE_H
