#ifndef ENTITYDELEGATEBUILDER_H
#define ENTITYDELEGATEBUILDER_H

#include "entitymodel.h"
#include "ientitymodelbuilder.h"

class EntityModelBuilder : public IEntityModelBuilder<DefaultModelInterface,DefaultFileModelList>
{
public:
    DefaultModelInterface *buildFileInformationModel(const QStringList &paths)
    {
        auto model = new FileInformationEntityModel();

        auto id = QUuid::createUuid();
        model->setId(id);

        auto type = EntityModelContext::FileInformationEntity;
        model->setType(type);

        model->setFilePaths(paths);

        return model;
    }

    DefaultModelInterface *buildDirectoryCountModel(const QString &path, const QString &name, const long long &size)
    {

        auto model = new DirectoryEntityModel();

        auto id = QUuid::createUuid();
        model->setId(id);

        auto type = EntityModelContext::DirectoryEntity;
        model->setType(type);

        model->setName(name);
        model->setPath(path);
        model->setSize(size);

        return model;
    }
    DefaultModelInterface *buildFileRuleModel(const DefaultFileModelList &files,
                                              int ruleAction,
                                              const QStringList &destinations)
    {
        auto model = new FileRuleEntityModel;

        auto id = QUuid::createUuid();
        model->setId(id);

        auto type = EntityModelContext::FileRuleEntity;
        model->setType(type);

        //model->setPaths(paths);
        model->setFiles(files);
        model->setRuleAction(ruleAction);
        model->setDestinations(destinations);

        return model;

    }
    DefaultModelInterface *buildErrorModel(const QString &errorMessage)
    {
        auto model = new ErrorEntityModel;

        auto id = QUuid::createUuid();
        model->setId(id);

        auto type = EntityModelContext::FileRuleEntity;
        model->setType(type);

        model->setErrorDescription(errorMessage);

        return model;
    }
};

#endif // ENTITYDELEGATEBUILDER_H
