#ifndef PROCESSCONTROLLER_H
#define PROCESSCONTROLLER_H

#include "mydatetime.h"
#include "fileinformationmanager.h"
#include "entitymodel.h"

/*
 * Queue management of fileoperations and other stuff
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
            emit sendEntity(new EntityModel("Queue is empty"));
            return;
        }

        emit sendEntity(entityQueue.takeFirst());
    }

signals:
    void wakeUpProcess();
    void processFinished();

    void sendEntity(EntityModel *entity);


private:
    QList<EntityModel*>entityQueue;
};

#endif // PROCESSCONTROLLER_H
