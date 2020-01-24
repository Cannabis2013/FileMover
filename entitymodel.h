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

    T* getModelValue() const
    {
        return static_cast<T*>(new EntityModel(*_model));
    }

    template<class t>
    t* getModelValue() const
    {
        return static_cast<t*>(new EntityModel(*_model));
    }

    EntityModel::typeMode type()
    {
        return _model->type;
    }

private:

    const EntityModel *_model;

    friend class EntityModelDelegateBuilder;
};

class EntityModelDelegateBuilder
{
public:
    static EntityModelDelegate<EntityModel>* buildDelegate(const EntityModel* entity)
    {
        return new EntityModelDelegate<EntityModel>(entity);
    }

    static EntityModelDelegate<EntityModel>* buildErrorEntity(const QString &err)
    {
        EntityModelDelegateBuilder builder;
        EntityModel *entity =
                builder.buildEntity<EntityModel>(EntityModel::nullEntity);
        entity->errorDescription = err;
        return new EntityModelDelegate<EntityModel>(entity);
    }
    template<class T>
    static EntityModelDelegate<T>* buildFileInformationEntity(const QStringList &paths)
    {
        EntityModelDelegateBuilder builder;
        if(!std::is_base_of_v<Model,T>)
            throw "Template class not direct base of Model";

        FileInformationEntity *entity =
                builder.buildEntity<FileInformationEntity>(EntityModel::fileInformationEntity);

        entity->filePaths = paths;

        return new EntityModelDelegate<T>(entity);
    }
    template<class T>
    static EntityModelDelegate<T>* buildFileActionEntity(const QStringList &dirPaths,
                                                     const FileObjectList &allFiles,
                                                     const rD::fileActionEntity &fileActionRule,
                                                     const QStringList &destinations)
    {
        if(!std::is_base_of_v<Model,T>)
            throw "Template class not direct base of Model";

        EntityModelDelegateBuilder builder;
        FileActionEntity *entity =
                builder.buildEntity<FileActionEntity>(EntityModel::fileActionEntity);

        entity->directoryPaths = dirPaths;
        entity->allFiles = allFiles;
        entity->fileActionRule = fileActionRule;
        entity->fileDestinations = destinations;

        return new EntityModelDelegate<T>(entity);
    }

    template<class T>
    static EntityModelDelegate<T>* buildDirectoryCountEntity(const long long &size,
                                                     const QString &name,
                                                     const QString &path)
    {
        if(!std::is_base_of_v<Model,T>)
            throw "Template class not direct base of Model";

        EntityModelDelegateBuilder builder;
        auto *entity =
                builder.buildEntity<DirectoryCountEntity>(EntityModel::directoryCountEntity);

        entity->directorySize = size;
        entity->directoryName = name;
        entity->directoryPath = path;

        return new EntityModelDelegate<T>(entity);
    }

private:
    template<class T = EntityModel>
    T *buildEntity(EntityModel::typeMode type)
    {
        if(!std::is_base_of_v<Model,T>)
            throw "Not base of EntityModel";

        EntityModel *entity = new T;
        entity->type = type;

        return static_cast<T*>(entity);
    }
};

#endif // ENTITYMODEL_H
