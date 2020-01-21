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
    enum typeMode {nullEntity,fileInformationEntity,fileActionEntity,directoryCountEntity};
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

template<class T>
class EntityModelDelegate : public IModelDelegate<T>
{
public:
    EntityModelDelegate(const EntityModel *model)
    {
        _model = model;
    }
    ~EntityModelDelegate()
    {
        delete _model;
    }

    quint64 modelId()
    {
        return _model->id;
    }

    const T *model() const
    {
        return static_cast<const T*>(_model);
    }

    template<class t>
    t* getModelValue() const
    {
        return static_cast<t*>(new EntityModel(*_model));
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
                makeEntity<FileActionEntity>(EntityModel::fileActionEntity);
        entity->directoryPaths = dirPaths;
        entity->allFiles = allFiles;
        entity->fileActionRule = fileActionRule;
        entity->fileDestinations = destinations;
        EntityModelDelegate *delegate = new EntityModelDelegate(entity);
        return delegate;
    }

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

    T* getModelValue() const
    {
        return static_cast<T*>(new EntityModel(*_model));
    }

    EntityModel::typeMode type()
    {
        return _model->type;
    }

private:
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
#endif // ENTITYMODEL_H
