#ifndef FILEMODELBUILDER_H
#define FILEMODELBUILDER_H

#include "imodelbuilder.h"
#include "ifilemodel.h"
#include "defaultfilemodelscontext.h"

class FileModelBuilder : public IModelBuilder<IFileModel<>,QString>
{
public:
    IFileModel<> *buildModel(const QString *modelArguments) override
    {
        auto model = new DefaultFileModel();
        model->setFilepath(*modelArguments);

        return model;
    }
};

#endif // FILEMODELBUILDER_H
