#ifndef ENTITYDELEGATE_H
#define ENTITYDELEGATE_H

#include "imodeldelegates.h"

class EntityModelDelegate : public IModelDelegate<DefaultModel,int>
{
public:
    ~EntityModelDelegate()
    {
        delete _model;
    }

    QUuid modelId() const {return _model->id();}
    const DefaultModel *model() const {return _model;}
    int type() const {return _model->type();}

private:
    EntityModelDelegate(const DefaultModel *model)
    {
        _model = model;
    }

    const DefaultModel *_model;

    friend class DelegateBuilder;
};


#endif // ENTITYDELEGATE_H
