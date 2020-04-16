#ifndef IMODELBUILDER_H
#define IMODELBUILDER_H



template<class TModel, class TModelArguments>
class IModelBuilder
{
public:
    virtual TModel *buildModel(const TModelArguments *modelArguments) = 0;
};

#endif // IMODELBUILDER_H
