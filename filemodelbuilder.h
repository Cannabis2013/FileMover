#ifndef FILEMODELBUILDER_H
#define FILEMODELBUILDER_H

#include "imodelbuilder.h"
#include "ifilemodel.h"

class FileModelBuilder : public IModelBuilder
{
public:


    // IModelBuilder interface
public:
    DefaultFileModel *buildModel(const DefaultFileModelArguments *modelArguments) override
    {
        auto model = new DefaultFileModel();

    }
};

#endif // FILEMODELBUILDER_H
