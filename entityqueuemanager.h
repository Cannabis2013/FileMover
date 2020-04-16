#ifndef PROCESSCONTROLLER_H
#define PROCESSCONTROLLER_H

#include "customdate.h"
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

    void addEntity(const DefaultModelInterface *model) override
    {
        entityQueue << model;
        emit wakeUpProcess();
    }

    void sendNextEntity() override
    {
        if(entityQueue.isEmpty())
        {
            return;
        }
        auto model = entityQueue.first();
        emit sendEntity(model);
    }

private:
    QList<const DefaultModelInterface*>entityQueue;

};

#endif // PROCESSCONTROLLER_H
