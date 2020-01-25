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
};

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

template<class T>
class EntityModelDelegate : public IModelDelegate<T>
{
public:
    ~EntityModelDelegate() {delete _model;}

    QUuid modelId(){return _model->id;}

    const T *model() const {return static_cast<const T*>(_model);}

    T* getModelValue() const {return static_cast<T*>(new EntityModel(*_model));}

    template<class t>
    t* getModelValue() const {return static_cast<t*>(new EntityModel(*_model));}

    EntityModel::typeMode type() {return _model->type;}

private:
    EntityModelDelegate(const EntityModel *model)
    {
        if(!std::is_base_of_v<EntityModel,T>)
            throw "Delegate can't be instantiated with class T not direct base of EntityModel";

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
            throw "Template class not direct base of Model";

        return new EntityModelDelegate<T>(entity);
    }

    template<class T = ErrorEntity>
    static EntityModelDelegate<T>* buildErrorEntity(const QString &err)
    {
        if(!std::is_base_of_v<EntityModel,T>)
            throw "Template class not direct base of Model";

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
            throw "Template class not direct base of Model";

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
            throw "Template class not direct base of Model";

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
            throw "Template class not direct base of Model";

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
    T *buildEntity(EntityModel::typeMode type)
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
