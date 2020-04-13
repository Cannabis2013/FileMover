#ifndef IMODEL_H
#define IMODEL_H

#include "quuid.h"

template<typename TModelType>
class IModel
{
public:
    virtual QUuid id() const = 0;
    virtual void setId(const QUuid &id) = 0;
    virtual TModelType type() const = 0;
    virtual void setType(const TModelType &type) = 0;
};

typedef IModel<int> DefaultModelInterface;

#endif // IMODEL_H
