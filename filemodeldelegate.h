#ifndef FILEOBJECTMODEL_H
#define FILEOBJECTMODEL_H

#include <QFileInfo>
#include <QString>
#include <QList>
#include "modeldelegates.h"


struct FileModel : public QFileInfo, public TreeModel{};

template<class T = FileModel>
class FileModelDelegate : public ITreeModelDelegate<T,DefaultModelType>
{
public:


    ITreeModelDelegate<T,DefaultModelType> *parentModelDelegate() const
    {
        return new FileModelDelegate(*static_cast<const FileModel*>(_model->_parent));
    }
    void setParentModelDelegate(ITreeModelDelegate<T,DefaultModelType> *const &newParent)
    {
        FileModelDelegate *delegate = dynamic_cast<FileModelDelegate*>(newParent);
        _model->_parent = delegate->_model;
    }

    QList<ITreeModelDelegate<T,DefaultModelType>*> children() const
    {
        QList<ITreeModelDelegate<T,DefaultModelType>*> delegates;
        for (const TreeModel *child : _model->_children)
            delegates << new FileModelDelegate(*static_cast<const FileModel*>(child));

        return delegates;
    }
    void setChildren(const QList<ITreeModelDelegate<T,DefaultModelType>*> &newChildren)
    {
        QList<TreeModel*> models;
        for (const ITreeModelDelegate<T,DefaultModelType> *child : newChildren)
        {
            const FileModelDelegate<T> *C = static_cast<const FileModelDelegate<T>*>(child);
            models << C->_model;
        }

        _model->_children = models;
    }
    void appendChild(ITreeModelDelegate<T,DefaultModelType> * const &object)
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

    static FileModelDelegate* buildDelegate(const QString &path)
    {
        return new FileModelDelegate(path);
    }

    FileModel *modelValue() const
    {
        return new FileModel(*_model);
    }

    DefaultModelType type()
    {
        return DefaultModelType::Tree;
    }

    QUuid modelId()
    {
        return _model->id;
    }
private:
    FileModelDelegate(const QString &path)
    {
        _model = new FileModel;
        _model->id = QUuid::createUuid();
        _model->setFile(path);
    }

    FileModelDelegate(const FileModel &model)
    {
        *_model = model;
    }

    FileModel *_model = new FileModel;

};
typedef QList<ITreeModelDelegate<FileModel,DefaultModelType>*> FileObjectList;
#endif // FILEOBJECTMODEL_H


