#ifndef FILEMODEL_H
#define FILEMODEL_H

#include "filemodeldelegate.h"

class FileModel :
        public IFileModel<FileModel>,
        public QFileInfo
{
public:
    const IFileModel<FileModel> *parent() const
    {
        return _parent;
    }
    void setParent(const IFileModel<FileModel> *parent)
    {
        _parent = parent;
    }
    QList<const FileModel *> children() const
    {
        QList<const FileModel*> c;
        for (const FileModel* child : _children)
            c << child;

        return c;
    }
    void setChildren(const QList<FileModel *> &children)
    {
        for (FileModel* child : children) {
            child->setParent(this);
            _children.append(child);
        }
    }

private:
    const IFileModel<FileModel> *_parent = nullptr;
    QList<FileModel*> _children;

};

class FileModelDelegate : public IFileModelDelegate<IFileModel<FileModel>>
{
public:
    FileModelDelegate(const FileModel &model)
    {
        _model = new FileModel(model);
    }

    const IFileModel<FileModel> *parentModel() const
    {
        const FileModel* m;
        try {
            m = model();
        } catch (const char* msg) {
            throw msg;
        }
        return m->parent();
    }
    void setParentModel(const IFileModelDelegate<IFileModel<FileModel>> *newParent)
    {
        const IFileModel<FileModel>* candidateParent;
        try {
            candidateParent = newParent->model();
        } catch (const char *msg) {
            throw msg;
        }
        _model->setParent(candidateParent);
    }

    const QList<const IFileModelDelegate<IFileModel<FileModel>> *> children() const
    {
        QList<const IFileModelDelegate<IFileModel<FileModel>>*> c;
        const FileModel *m;
        try {
            m = model();
        } catch (const char *msg) {
            throw msg;
        }
        for (const FileModel *model : m->children())
            c << new FileModelDelegate(*model);

        return c;
    }
    void setChildren(const QList<const IFileModelDelegate<IFileModel<FileModel>> *> &value)
    {
        QList<FileModel*> children;
        for (const IFileModelDelegate<IFileModel<FileModel>> *delegate : value)
        {
            FileModel *object = static_cast<FileModel*>(delegate->M());
            const FileModel *m;
            try {
                m = model();
            } catch (const char *msg) {
                throw msg;
            }
            object->setParent(m);
            children << object;
        }

        _model->setChildren(children);
    }
    void appendChild(const IFileModelDelegate<IFileModel<FileModel>> *object)
    {
        IFileModel<FileModel> *model = object->M();
        model->setParent(_model);
        _model->children().append(static_cast<FileModel*>(model));
    }
    bool hasChildren()
    {
        return !_model->children().isEmpty();
    }

    QString parentFolderName() const
    {
        if(_model->parent() == nullptr)
            return QString();

        const FileModel *parent = dynamic_cast<const FileModel*>(parentModel());
        return parent->fileName();
    }

    const FileModel *model() const
    {
        if(std::is_base_of_v<IFileModel<FileModel>, FileModel>)
            return static_cast<FileModel*>(_model);
        else
            throw "Is not direct base of";
    }

private:

    IFileModel<FileModel>* M() const
    {
        return _model;
    }

    FileModel *_model;
};

#endif // FILEMODEL_H
