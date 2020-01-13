#ifndef PROCESSCONTROLLER_H
#define PROCESSCONTROLLER_H

#include "mydatetime.h"
#include "fileinformationmanager.h"
#include "entitymodel.h"
#include <qmutex.h>

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

    void addEntity(EntityModel *entity)
    {
        entityQueue << entity;
        emit wakeUpProcess();
    }

    void sendNextEntity()
    {
        if(entityQueue.isEmpty())
        {
            auto emptyEntity = eMD::makeEntity<EntityModel>(EntityModel::nullEntity);
            emptyEntity->errorDescription = "Queue is empty";
            emit sendEntity(emptyEntity);
            return;
        }
        auto firstEntity = entityQueue.takeFirst();
        emit sendEntity(firstEntity);
    }

signals:
    void wakeUpProcess();
    void processFinished();

    void sendEntity(EntityModel *entity);

private:
    QList<EntityModel*>entityQueue;

};

#endif // PROCESSCONTROLLER_H
