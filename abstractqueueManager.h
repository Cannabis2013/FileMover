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
    virtual void addEntity(IModelDelegate<EntityModel,DefaultEntityType> *delegate) = 0;
    virtual void sendNextEntity() = 0;

signals:
    void wakeUpProcess();
    void processFinished();
    void sendEntity(IModelDelegate<EntityModel,DefaultEntityType> *delegate);
};


#endif // ABSTRACTQUEUEMANAGER_H
