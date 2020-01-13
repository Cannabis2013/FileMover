#ifndef ENTITYMODEL_H
#define ENTITYMODEL_H

#include "ruledefinitions.h"
#include "filemodeldelegate.h"
#include <QFileInfoList>

#include <iostream>
#include "modeldelegates.h"

using namespace std;

struct EntityModel : public Model
{
    enum typeMode {nullEntity,fileInformationEntity,fileOperationEntity,directoryCountEntity};
    typeMode type = nullEntity;
    QString errorDescription = "No error";
};

struct DirectoryCountEntity : public EntityModel
{
    long long directorySize;
    QString directoryName;
    QString directoryPath;
};

struct FileActionEntity : public EntityModel
{
    QStringList directoryPaths;
    FileObjectList allFiles;
    rD::fileActionEntity fileActionRule = rD::none;
    QStringList fileDestinations = QStringList();
};

struct FileInformationEntity : public EntityModel
{
    QStringList filePaths;
};

class EntityModelDelegate : public IModelDelegate
{
public:
    ~EntityModelDelegate()
    {
        delete _model;
    }

    quint64 modelId()
    {
        return _model->id;
    }
    const EntityModel *model() const
    {
        return _model;
    }

    static EntityModelDelegate* make(const EntityModel* entity)
    {
        return new EntityModelDelegate(entity);
    }

    static EntityModelDelegate* makeErrorEntity(const QString &err)
    {
        EntityModel *entity =
                makeEntity<EntityModel>(EntityModel::nullEntity);
        entity->errorDescription = err;
        EntityModelDelegate *delegate = new EntityModelDelegate(entity);
        return delegate;
    }

    static EntityModelDelegate* makeFileInformationEntity(const QStringList &paths)
    {
        FileInformationEntity *entity =
                makeEntity<FileInformationEntity>(EntityModel::fileInformationEntity);
        entity->filePaths = paths;
        EntityModelDelegate *delegate = new EntityModelDelegate(entity);
        return delegate;
    }

    static EntityModelDelegate* makeFileActionEntity(const QStringList &dirPaths,
                                                     const FileObjectList &allFiles,
                                                     const rD::fileActionEntity &fileActionRule,
                                                     const QStringList &destinations)
    {
        FileActionEntity *entity =
                makeEntity<FileActionEntity>(EntityModel::fileOperationEntity);
        entity->directoryPaths = dirPaths;
        entity->allFiles = allFiles;
        entity->fileActionRule = fileActionRule;
        entity->fileDestinations = destinations;
        EntityModelDelegate *delegate = new EntityModelDelegate(entity);
        return delegate;
    }
    long long directorySize;
    QString directoryName;
    QString directoryPath;

    static EntityModelDelegate* makeDirectoryCountEntity(const long long &size,
                                                     const QString &name,
                                                     const QString &path)
    {
        DirectoryCountEntity *entity =
                makeEntity<DirectoryCountEntity>(EntityModel::directoryCountEntity);
        entity->directorySize = size;
        entity->directoryName = name;
        entity->directoryPath = path;
        EntityModelDelegate *delegate = new EntityModelDelegate(entity);
        return delegate;
    }

private:
    EntityModelDelegate(const EntityModel *model)
    {
        _model = model;
    }

    template<class Y>
    static Y *makeEntity(EntityModel::typeMode type)
    {
        if(!std::is_base_of_v<EntityModel,Y>)
            throw "Not base of EntityModel";

        EntityModel *entity = new Y;
        entity->type = type;

        return static_cast<Y*>(entity);
    }

    const EntityModel *_model;
};

typedef EntityModelDelegate eMD;
#endif // ENTITYMODEL_H
