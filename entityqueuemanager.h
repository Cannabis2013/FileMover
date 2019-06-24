#ifndef PROCESSCONTROLLER_H
#define PROCESSCONTROLLER_H

#include "mydatetime.h"
#include "fileinformationmanager.h"
#include "ruledefinitions.h"
#include "entitymodel.h"

/*
 * These classes handles the queues.
*/
class DirectoryEntity : public EntityModel
{
public:
    void setContentSize(long long size){sz = size;}
    void setDirectoryName(QString name){dirName = name;}
    void setDirectoryPath(QString path){this->path = path;}

    long long size(){return sz;}
    QString directoryName(){return dirName;}
    QString directoryPath(){return path;}

private:
    long long sz;
    QString dirName;
    QString path;
};
class FileActionEntity : public EntityModel
{
public:
    FileActionEntity()
    {
        setType(entityType::FileAction);
    }
    void setDirectoryPaths(QStringList paths){dirPaths = paths;}
    void setDirectoryFileContent(QFileInfoList list){this->list = list;}
    void setFileActionRule(rD::fileActionRule aRule){ruleMode = aRule;}
    void setFileActionDestinations(QStringList paths){destinations = paths;}

    QStringList directoryPaths(){return dirPaths;}
    QFileInfoList directoryFileList(){return list;}
    rD::fileActionRule fileActionRule(){return ruleMode;}
    QStringList fileActionDestinations(){return destinations;}

private:
    QStringList dirPaths;
    QFileInfoList list;
    rD::fileActionRule ruleMode = rD::none;
    QStringList destinations = QStringList();
};

class EntityQueueManager : public QObject
{
    Q_OBJECT

public:
    ~EntityQueueManager()
    {
        delete this;
    }

    void addEntity(EntityModel *entity)
    {
        entityQueue << entity;
        emit wakeUpProcess();
    }

    EntityModel *takeNextEntity(entityType tp)
    {
        for (int i = 0;i < entityQueue.count();i++)
        {
            EntityModel *model = entityQueue.at(i);
            if(model->entityType() == tp)
                return entityQueue.takeAt(i);
        }
        return nullptr;
    }

    bool isQueueEmpty(){return entityQueue.isEmpty();}

signals:
    void wakeUpProcess();
    void processFinished();


private:


    QList<EntityModel*>entityQueue;

    friend class FileWorker;
};

#endif // PROCESSCONTROLLER_H
