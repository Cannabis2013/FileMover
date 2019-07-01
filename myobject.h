#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <qobject.h>

class MyObject : public QObject
{
    Q_OBJECT
public:
    MyObject()
    {
    }

signals:
    void stateChanged();

};

#endif // MYOBJECT_H
