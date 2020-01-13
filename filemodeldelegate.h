#ifndef FILEOBJECTMODEL_H
#define FILEOBJECTMODEL_H

#include <QFileInfo>
#include <QString>
#include <QList>
#include "modeldelegates.h"


struct FileModel : public QFileInfo, public TreeModel{};

class FileModelDelegate : public ITreeModelDelegate
{
public:


    ITreeModelDelegate *parentModelDelegate() const
    {
        return new FileModelDelegate(*static_cast<const FileModel*>(_model->_parent));
    }
    void setParentModelDelegate(ITreeModelDelegate *const &newParent)
    {
        FileModelDelegate *delegate = dynamic_cast<FileModelDelegate*>(newParent);
        _model->_parent = delegate->_model;
    }

    QList<ITreeModelDelegate*> children() const
    {
        QList<ITreeModelDelegate*> delegates;
        for (const TreeModel *child : _model->_children)
            delegates << new FileModelDelegate(*static_cast<const FileModel*>(child));

        return delegates;
    }
    void setChildren(const QList<ITreeModelDelegate*> &newChildren)
    {
        QList<TreeModel*> models;
        for (const ITreeModelDelegate *child : newChildren)
        {
            const FileModelDelegate *C = static_cast<const FileModelDelegate*>(child);
            models << C->_model;
        }

        _model->_children = models;
    }
    void appendChild(ITreeModelDelegate * const &object)
    {
        FileModelDelegate *delegates = dynamic_cast<FileModelDelegate*>(object);
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
typedef FileModelDelegate fMD;
typedef QList<ITreeModelDelegate*> FileObjectList;
#endif // FILEOBJECTMODEL_H


