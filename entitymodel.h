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

template<class T = EntityModel>
class EntityModelDelegate : public IModelDelegate
{
public:
    quint64 modelId()
    {
        return _model->id;
    }
    const T *model() const
    {
        return _model;
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

private:
    EntityModelDelegate(const T &model)
    {
        *_model = model;
    }


    T *_model = new T;
};

typedef EntityModelDelegate<> eMD;

#endif // ENTITYMODEL_H
