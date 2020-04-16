#ifndef FILEMODELBUILDER_H
#define FILEMODELBUILDER_H

#include "imodelbuilder.h"
#include "ifilemodel.h"
#include "defaultfilemodelscontext.h"


class FileModelBuilder : public IModelBuilder<IFileModel<QFileInfo,QUuid>,QString>
{
public:
    IFileModel<QFileInfo,QUuid> *buildModel(const QString *modelArguments) override
    {
        auto model = new FileModel();
        model->setFilepath(*modelArguments);


        return model;
    }
};

#endif // FILEMODELBUILDER_H
