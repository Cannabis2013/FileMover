#ifndef MODELDELEGATES_H
#define MODELDELEGATES_H

#include <qlist.h>
#include <quuid.h>

struct Model
{
    QUuid id;
};

struct TreeModel : public Model
{
    TreeModel *_parent = nullptr;
    QList<TreeModel*>_children;
};

template<class T>
class IModelDelegate
{
public:
    IModelDelegate() = default;
    virtual ~IModelDelegate() = default;
    virtual QUuid modelId() = 0;
    virtual const T* model() const= 0;
};
template<class T = TreeModel>
class ITreeModelDelegate : public IModelDelegate<T>
{
public:
    virtual ITreeModelDelegate *parentModelDelegate() const = 0;
    virtual void setParentModelDelegate(ITreeModelDelegate *const &newParent) = 0;
    virtual QList<ITreeModelDelegate*> children() const = 0;
    virtual void setChildren(const QList<ITreeModelDelegate*> &newChildren)=0;
    virtual void appendChild(ITreeModelDelegate *const &object)=0;
    virtual bool hasChildren() const = 0;
};

#endif // MODELDELEGATES_H
