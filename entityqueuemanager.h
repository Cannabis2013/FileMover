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
            // TODO: You have to make sure that FileWorker knows this state has been reached

            auto model = new EntityModel();
            model->setType(EntityModelContext::NullEntity);

            emit sendEntity(model);

            return;
        }
        auto model = entityQueue.takeFirst();
        emit sendEntity(model);
    }

private:
    QList<const DefaultModelInterface*>entityQueue;
};

#endif // PROCESSCONTROLLER_H
