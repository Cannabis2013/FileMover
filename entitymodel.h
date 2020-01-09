#ifndef ENTITYMODEL_H
#define ENTITYMODEL_H

#include "ruledefinitions.h"
#include "filemodel.h"
#include <QFileInfoList>

#include <iostream>

using namespace std;


struct EntityModel
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
    QList<const IFileModelDelegate<IFileModel<FileModel>>*> allFiles;
    rD::fileActionEntity fileActionRule = rD::none;
    QStringList fileDestinations = QStringList();
};

struct FileInformationEntity : public EntityModel
{
    QStringList filePaths;
};

template<class T> T *makeEntity(EntityModel::typeMode type)
{
    if(!std::is_base_of_v<EntityModel,T>)
        throw "Not base of EntityModel";

    EntityModel *entity = new T;
    entity->type = type;

    return static_cast<T*>(entity);
}

#endif // ENTITYMODEL_H
