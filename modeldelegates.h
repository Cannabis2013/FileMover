#ifndef MODELDELEGATES_H
#define MODELDELEGATES_H

#include <qlist.h>

struct Model
{
    quint64 id;
};

struct TreeModel : public Model
{
    TreeModel *_parent = nullptr;
    QList<TreeModel*>_children;
};
class IModelDelegate
{
public:
    IModelDelegate() = default;
    virtual ~IModelDelegate() = default;
    virtual quint64 modelId() = 0;
};
class ITreeModelDelegate : public IModelDelegate
{
public:
    virtual ITreeModelDelegate *parentModelDelegate() const = 0;
    virtual void setParentModelDelegate(ITreeModelDelegate *const &newParent) = 0;
    virtual QList<ITreeModelDelegate*> children() const = 0;
    virtual void setChildren(const QList<ITreeModelDelegate*> &newChildren)=0;
    virtual void appendChild(ITreeModelDelegate *const &object)=0;
    virtual bool hasChildren() const = 0;
    virtual const TreeModel *model() const = 0;
};

#endif // MODELDELEGATES_H
