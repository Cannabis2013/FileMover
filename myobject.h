#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <qobject.h>

/*
 * Provides the 'notify observers' and message features.
 */

class MyObject : public QObject
{
    Q_OBJECT
public:
    MyObject(){}

signals:
    void stateChanged();

    void sendSystemTrayMessage(const QString &title,const QString &msg);
    void sendStatusLineMessage(const QString &msg);

};

#endif // MYOBJECT_H
