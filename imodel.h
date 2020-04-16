#ifndef IMODEL_H
#define IMODEL_H

template<typename TModelType, typename TUniqueId>
class IModel
{
public:
    virtual TUniqueId id() const = 0;
    virtual void setId(const TUniqueId &id) = 0;
    virtual TModelType type() const = 0;
    virtual void setType(const TModelType &type) = 0;
};


#endif // IMODEL_H
