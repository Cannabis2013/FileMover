#ifndef ENTITYMODEL_H
#define ENTITYMODEL_H

#include "ruledefinitions.h"
#include "filemodeldelegate.h"
#include <QFileInfoList>

#include <iostream>
#include "modeldelegates.h"

#define THROW_MSG_INHERIT "Template argument not direct base of Model"

using namespace std;

struct EntityModel : public Model
{
    enum typeMode {nullEntity,fileInformationEntity,fileActionEntity,directoryCountEntity};
    typeMode type = nullEntity;
};

typedef EntityModel::typeMode EntityType;

struct ErrorEntity : public EntityModel
{
    QString errorDescription = "No error";
};

struct DirectoryEntity : public EntityModel
{
    long long directorySize;
    QString directoryName;
    QString directoryPath;
};

struct FileRuleEntity : public EntityModel
{
    QStringList directoryPaths;
    FileObjectList allFiles;
    rD::ruleAction fileActionRule = rD::none;
    QStringList fileDestinations = QStringList();
};

struct FileInformationEntity : public EntityModel
{
    QStringList filePaths;
};

template<class ModelType>
class EntityModelDelegate : public IModelDelegate<ModelType,EntityType>
{
public:
    ~EntityModelDelegate()
    {
        delete _model;
    }

    QUuid modelId(){return _model->id;}

    const ModelType *model() const {return static_cast<const ModelType*>(_model);}

    ModelType* modelValue() const {return static_cast<ModelType*>(new EntityModel(*_model));}

    template<class t>
    t* modelValue() const {return static_cast<t*>(new EntityModel(*_model));}

    EntityType type() {return _model->type;}

private:
    EntityModelDelegate(const EntityModel *model)
    {
        if(!std::is_base_of_v<EntityModel,ModelType>)
            throw THROW_MSG_INHERIT;

        _model = model;
    }

    const EntityModel *_model;

    friend class DelegateBuilder;
};

class DelegateBuilder
{
public:
    template<class T = EntityModel>
    static EntityModelDelegate<T>* buildDelegate(const EntityModel* entity)
    {
        if(!std::is_base_of_v<EntityModel,T>)
            throw THROW_MSG_INHERIT;

        return new EntityModelDelegate<T>(entity);
    }

    template<class T = ErrorEntity>
    static EntityModelDelegate<T>* buildErrorEntity(const QString &err)
    {
        if(!std::is_base_of_v<EntityModel,T>)
            throw THROW_MSG_INHERIT;

        DelegateBuilder builder;
        auto entity =
                builder.buildEntity<ErrorEntity>(EntityModel::nullEntity);

        entity->errorDescription = err;
        return new EntityModelDelegate<T>(entity);
    }
    template<class T = FileInformationEntity>
    static EntityModelDelegate<T>* buildFileInformationEntity(const QStringList &paths)
    {
        DelegateBuilder builder;
        if(!std::is_base_of_v<EntityModel,T>)
            throw THROW_MSG_INHERIT;

        FileInformationEntity *entity =
                builder.buildEntity<FileInformationEntity>(EntityModel::fileInformationEntity);

        entity->filePaths = paths;

        return new EntityModelDelegate<T>(entity);
    }
    template<class T>
    static EntityModelDelegate<T>* buildFileActionEntity(const QStringList &dirPaths,
                                                     const FileObjectList &allFiles,
                                                     const rD::ruleAction &fileActionRule,
                                                     const QStringList &destinations)
    {
        if(!std::is_base_of_v<EntityModel,T>)
            throw THROW_MSG_INHERIT;

        DelegateBuilder builder;
        FileRuleEntity *entity =
                builder.buildEntity<FileRuleEntity>(EntityModel::fileActionEntity);

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
        if(!std::is_base_of_v<EntityModel,T>)
            throw THROW_MSG_INHERIT;

        DelegateBuilder builder;
        auto *entity =
                builder.buildEntity<DirectoryEntity>(EntityModel::directoryCountEntity);

        entity->directorySize = size;
        entity->directoryName = name;
        entity->directoryPath = path;

        return new EntityModelDelegate<T>(entity);
    }

private:
    template<class T>
    T *buildEntity(EntityType type)
    {
        if(!std::is_base_of_v<EntityModel,T>)
            throw "Class Not direct base of EntityModel";

        EntityModel *entity = new T;
        entity->type = type;
        entity->id = QUuid::createUuid();

        return static_cast<T*>(entity);
    }
};

#endif // ENTITYMODEL_H
