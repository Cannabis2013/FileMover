#ifndef PROCESSCONTROLLER_H
#define PROCESSCONTROLLER_H

#include "mydatetime.h"
#include "fileinformationmanager.h"
#include "exceptionhandler.h"
#include "abstractqueueManager.h"

/*
 * Queue management of fileoperations and other stuff
 *
 * signals and slot's call flow:
 *  - EntityQueueManager::WakeupProcess -> FileOperationWorker::HandleProcessRequest
 *  - FileOperationWorker::requestNextEntity -> EntityQueueManager::sendNextEntity
 *  - EntityQueueManager::SendEntity -> FileWorkerOperation::processEntity
*/

class EntityQueueManager : public AbstractQueueManager
{
    Q_OBJECT
public:
    ~EntityQueueManager()
    {
        delete this;
    }

    bool isQueueEmpty() const override {return entityQueue.isEmpty();}

public slots:

    void addEntity(IModelDelegate<EntityModel,EntityType> *delegate) override
    {
        entityQueue << delegate->model();
        emit wakeUpProcess();
    }

    void sendNextEntity() override
    {
        if(entityQueue.isEmpty())
        {
            emit sendEntity(DelegateBuilder::buildErrorEntity<EntityModel>("Queue is empty"));
            return;
        }

        auto delegate = DelegateBuilder::buildDelegate(entityQueue.takeFirst());
        emit sendEntity(delegate);
    }

private:
    QList<const EntityModel*>entityQueue;

};

#endif // PROCESSCONTROLLER_H
