#ifndef ABSTRACTFRAMEIMPLEMENTABLE_H
#define ABSTRACTFRAMEIMPLEMENTABLE_H

#include <qwidget.h>
#include <QMouseEvent>
#include "imutableobject.h"

class AbstractFrame :
        public QWidget,
        public IMutableObject
{
    Q_OBJECT
public:
    AbstractFrame(QWidget *parent = nullptr);

    bool resizeable(){return _resizeable;}
    void setResizeable(bool resizeMode){_resizeable = resizeMode;}

    void setWidgetTitle(QString title);

    QString widgetTitle() {return _widgetTitle;}

signals:
    void sizeChanged(QSize newSize);
    void stateChanged();

protected:

    // Reimplemented
    void closeEvent(QCloseEvent *event);

    // Pure virtual
    virtual void resizeNotify(QSize newSize) = 0;

private:
    bool _resizeable = true;
    QString _widgetTitle = "Title";

public:
};

#endif // ABSTRACTFRAMEIMPLEMENTABLE_H
