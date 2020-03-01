#ifndef THREADSMANAGER_H
#define THREADSMANAGER_H

#include <QThread>
#include <qlist.h>
#include <quuid.h>

#include "ithreadmanagerinterface.h"

class ThreadObject : public QThread
{
public:
    ThreadObject(QObject *parent = nullptr):
        QThread(parent),_thread_id(QUuid::createUuid()){}
    QUuid thread_id() const;

private:
    const QUuid _thread_id;
};

class ThreadsManager : public IThreadManagerInterface
{
public:
    ThreadsManager();
    ~ThreadsManager();

    const ThreadDescriptor *createThread(QObject *o) override;
    void startThread(QUuid thread_id, QThread::Priority priority = QThread::InheritPriority) override;
    void terminateThread(QUuid thread_id) override;
    void startAllThreads(QThread::Priority priority = QThread::InheritPriority) override;
    void terminateAll() override;
private:
    QList<QThread*> _threads;
};

#endif // THREADSMANAGER_H
