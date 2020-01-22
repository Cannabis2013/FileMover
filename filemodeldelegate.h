#ifndef FILEOBJECTMODEL_H
#define FILEOBJECTMODEL_H

#include <QFileInfo>
#include <QString>
#include <QList>
#include "modeldelegates.h"


struct FileModel : public QFileInfo, public TreeModel{};

template<class T = FileModel>
class FileModelDelegate : public ITreeModelDelegate<T>
{
public:


    ITreeModelDelegate<T> *parentModelDelegate() const
    {
        return new FileModelDelegate(*static_cast<const FileModel*>(_model->_parent));
    }
    void setParentModelDelegate(ITreeModelDelegate<T> *const &newParent)
    {
        FileModelDelegate *delegate = dynamic_cast<FileModelDelegate*>(newParent);
        _model->_parent = delegate->_model;
    }

    QList<ITreeModelDelegate<T>*> children() const
    {
        QList<ITreeModelDelegate<T>*> delegates;
        for (const TreeModel *child : _model->_children)
            delegates << new FileModelDelegate(*static_cast<const FileModel*>(child));

        return delegates;
    }
    void setChildren(const QList<ITreeModelDelegate<T>*> &newChildren)
    {
        QList<TreeModel*> models;
        for (const ITreeModelDelegate<T> *child : newChildren)
        {
            const FileModelDelegate<T> *C = static_cast<const FileModelDelegate<T>*>(child);
            models << C->_model;
        }

        _model->_children = models;
    }
    void appendChild(ITreeModelDelegate<T> * const &object)
    {
        FileModelDelegate<T> *delegates = dynamic_cast<FileModelDelegate<T>*>(object);
        object->setParentModelDelegate(this);
        _model->_children.append(delegates->_model);
    }

    bool hasChildren() const{return !_model->_children.isEmpty();}

    const FileModel * model() const
    {
        return _model;
    }

    static FileModelDelegate* delegateFromPath(const QString &path)
    {
        return new FileModelDelegate(path);
    }

    quint64 modelId()
    {
        return _model->id;
    }
private:
    FileModelDelegate(const QString &path)
    {
        _model = new FileModel;
        _model->id = 666;
        _model->setFile(path);
    }

    FileModelDelegate(const FileModel &model)
    {
        *_model = model;
    }

    FileModel *_model = new FileModel;

};
typedef QList<ITreeModelDelegate<FileModel>*> FileObjectList;
#endif // FILEOBJECTMODEL_H


