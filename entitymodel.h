#ifndef ENTITYMODEL_H
#define ENTITYMODEL_H

#include "ruledefinitions.h"

class EntityModel
{
public:
    EntityModel();
    enum typeMode {informationAction,FileAction,directoryCountAction};

    void setType(typeMode type){this->type = type;}
    typeMode entityType(){return type;}

private:
    typeMode type;
};

typedef EntityModel::typeMode entityType;

class DirectoryCountEntity : public EntityModel
{
public:
    DirectoryCountEntity()
    {
        setType(entityType::directoryCountAction);
    }

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

class fileInformationEntity : public EntityModel
{
public:
    fileInformationEntity()
    {
        setType(entityType::informationAction);
    }
    fileInformationEntity(QStringList paths)
    {
        setType(entityType::informationAction);

        p = paths;
    }
    fileInformationEntity(QString path)
    {
        setType(entityType::informationAction);

        p << path;
    }

    void addPath(QStringList path){p << path;}
    void addPath(QString path){p << path;}

    QStringList paths(){return p;}
    const QString pathAt(int index){return p.at(index);}

    int pathCount(){return p.count();}

private:
    QStringList p;
};

#endif // ENTITYMODEL_H
