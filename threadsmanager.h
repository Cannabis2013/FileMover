#ifndef THREADSMANAGER_H
#define THREADSMANAGER_H

#include <QThread>
#include <qlist.h>
#include <quuid.h>

struct ThreadDescriptor
{
    const QThread * thread;
    QUuid thread_id;
};

class ThreadObject : public QThread
{
public:
    ThreadObject(QObject *parent = nullptr):
        QThread(parent),_thread_id(QUuid::createUuid()){}
    QUuid thread_id() const;

private:
    const QUuid _thread_id;
};

class ThreadsManager
{
public:
    ThreadsManager();
    ~ThreadsManager();

    const ThreadDescriptor *createThread(QObject *o);
    void startThread(QUuid thread_id, QThread::Priority priority = QThread::InheritPriority);
    void terminateThread(QUuid thread_id);
    void startAllThreads(QThread::Priority priority = QThread::InheritPriority);
    void terminateAll();
private:
    QList<QThread*> _threads;
};

#endif // THREADSMANAGER_H
