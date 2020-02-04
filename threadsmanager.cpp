#include "threadsmanager.h"

ThreadsManager::ThreadsManager()
{
}

ThreadsManager::~ThreadsManager()
{
    terminateAll();
}

const ThreadDescriptor *ThreadsManager::createThread(QObject * o)
{
    auto descriptor = new ThreadDescriptor();

    QThread *t = new ThreadObject();
    o->moveToThread(t);

    descriptor->thread = t;
    descriptor->thread_id = dynamic_cast<ThreadObject*>(t)->thread_id();
    _threads.append(t);

    return descriptor;
}

void ThreadsManager::startThread(QUuid thread_id,QThread::Priority priority)
{
    for (auto t : _threads) {
        if(dynamic_cast<ThreadObject*>(t)->thread_id() == thread_id)
            t->start();
    }
}

void ThreadsManager::terminateThread(QUuid thread_id)
{
    for (auto t : _threads) {
        if(dynamic_cast<ThreadObject*>(t)->thread_id() == thread_id)
            t->terminate();
    }
}

void ThreadsManager::startAllThreads(QThread::Priority priority)
{
    for (auto t : _threads)
        t->start(priority);
}
void ThreadsManager::terminateAll()
{
    for (auto t : _threads)
        t->terminate();
}


QUuid ThreadObject::thread_id() const
{
    return _thread_id;
}
