#ifndef PROCESSCONTROLLER_H
#define PROCESSCONTROLLER_H

#include "mydatetime.h"
#include "fileinformationmanager.h"
#include "entitymodel.h"
#include "exceptionhandler.h"

/*
 * Queue management of fileoperations and other stuff
 *
 * signals and slot's call flow:
 *  - EntityQueueManager::WakeupProcess -> FileOperationWorker::HandleProcessRequest
 *  - FileOperationWorker::requestNextEntity -> EntityQueueManager::sendNextEntity
 *  - EntityQueueManager::SendEntity -> FileWorkerOperation::processEntity
*/

class EntityQueueManager : public QObject
{
    Q_OBJECT

public:
    ~EntityQueueManager()
    {
        delete this;
    }

    bool isQueueEmpty(){return entityQueue.isEmpty();}

public slots:

    void addEntity(IModelDelegate<EntityModel,EntityType> *delegate)
    {
        entityQueue << delegate->model();
        emit wakeUpProcess();
    }

    void sendNextEntity()
    {
        if(entityQueue.isEmpty())
        {
            emit sendEntity(DelegateBuilder::buildErrorEntity<EntityModel>("Queue is empty"));
            return;
        }

        auto delegate = DelegateBuilder::buildDelegate(entityQueue.takeFirst());
        emit sendEntity(delegate);
    }

signals:
    void wakeUpProcess();
    void processFinished();

    void sendEntity(IModelDelegate<EntityModel,EntityType> *delegate);

private:
    QList<const EntityModel*>entityQueue;

};

#endif // PROCESSCONTROLLER_H
