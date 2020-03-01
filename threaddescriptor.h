#ifndef THREADDESCRIPTOR_H
#define THREADDESCRIPTOR_H

#include <QThread>
#include <quuid.h>

struct ThreadDescriptor
{
    const QThread * thread;
    QUuid thread_id;
};

#endif // THREADDESCRIPTOR_H
