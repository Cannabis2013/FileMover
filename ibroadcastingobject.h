#ifndef IBROADCASTINGOBJECT_H
#define IBROADCASTINGOBJECT_H

#include <qstring.h>

class IBroadcastingObject
{
public:
    virtual void sendSystemTrayMessage(const QString &title,const QString &msg) = 0;
    virtual void sendStatusLineMessage(const QString &msg) = 0;
};

#endif // IBROADCASTINGOBJECT_H
