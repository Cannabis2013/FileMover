#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <qobject.h>
#include <iostream>

using namespace std;

/*
 * Provides system tray notification service
 * Provides the 'notify observers' service
 */

class MutableObject : public QObject
{
    Q_OBJECT
signals:
    void stateChanged(); // Notify observers
    void sendSystemTrayMessage(const QString &title,const QString &msg);
    void sendStatusLineMessage(const QString &msg);
};

#endif // MYOBJECT_H

