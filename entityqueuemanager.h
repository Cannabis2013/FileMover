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

    EntityModel *nextEntity()
    {
        if(entityQueue.isEmpty())
            return nullptr;
        else
            return entityQueue.takeFirst();
    }

    bool isQueueEmpty(){return entityQueue.isEmpty();}

public slots:
    void addEntity(EntityModel *entity)
    {
        entityQueue << entity;
        emit wakeUpProcess();
    }

signals:
    void wakeUpProcess();
    void processFinished();


private:
    QList<EntityModel*>entityQueue;
};

#endif // PROCESSCONTROLLER_H
