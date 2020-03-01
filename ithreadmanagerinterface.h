#ifndef ABSTRACTTHREADMANAGER_H
#define ABSTRACTTHREADMANAGER_H

#include "threaddescriptor.h"

class IThreadManagerInterface
{
public:
    ~IThreadManagerInterface() = default;
    virtual const ThreadDescriptor *createThread(QObject *o) = 0;
    virtual void startThread(QUuid thread_id, QThread::Priority priority = QThread::InheritPriority) = 0;
    virtual void terminateThread(QUuid thread_id) = 0;
    virtual void startAllThreads(QThread::Priority priority = QThread::InheritPriority) = 0;
    virtual void terminateAll() = 0;
};

#endif // ABSTRACTTHREADMANAGER_H
