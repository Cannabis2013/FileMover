#ifndef FILEOBJECTMODEL_H
#define FILEOBJECTMODEL_H

#include "ifilemodel.h"
#include "quuid.h"

class FileModel : public IFileModel<QFileInfo,QUuid>
{
public:
    // IFileModel interface
    QString filepath() const override
    {
        return _filepath;
    }
    void setFilepath(const QString &path) override
    {
        _filepath = path;
    }

    const QList<const IFileModel<QFileInfo,QUuid> *> children() const override
    {
        return _children;
    }
    void setChildren(const QList<const IFileModel<QFileInfo,QUuid> *> &children) override
    {
        _children = children;
    }

    IFileModel<QFileInfo,QUuid> *parent() const override
    {
        return _parent;
    }
    void setParent(IFileModel<QFileInfo,QUuid> *parent) override
    {
        _parent = parent;
    }
    QFileInfo fileInterface() const  override
    {
        return QFileInfo(filepath());
    }

    // IModel interface
    QUuid id() const override
    {
        return _id;
    }
    void setId(const QUuid &id) override
    {
        _id = id;
    }
    int type() const override
    {
        return _type;
    }
    void setType(const int &type) override
    {
        _type = type;
    }

private:
    QString _filepath;
    IFileModel<QFileInfo,QUuid> *_parent = nullptr;
    QList<const IFileModel<QFileInfo,QUuid>*>_children;
    QUuid _id;
    int _type;
};




#endif // FILEOBJECTMODEL_H






