#ifndef ABSTRACTQUEUEMANAGER_H
#define ABSTRACTQUEUEMANAGER_H

#include "entitymodel.h"

class AbstractQueueManager : public QObject
{
    Q_OBJECT
public:
    ~AbstractQueueManager() = default;
    virtual bool isQueueEmpty() const = 0;

public slots:
    virtual void addEntity(const DefaultModelInterface *model) = 0;
    virtual void sendNextEntity() = 0;

signals:
    void wakeUpProcess();
    void processFinished();
    void sendEntity(const DefaultModelInterface *model);
};


#endif // ABSTRACTQUEUEMANAGER_H
