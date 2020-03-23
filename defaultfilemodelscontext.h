#ifndef FILEOBJECTMODEL_H
#define FILEOBJECTMODEL_H

#include "ifilemodel.h"

class DefaultFileModel : public IFileModel<QFileInfo>
{
public:
    QString filepath() const override
    {
        return _filepath;
    }
    void setFilepath(const QString &path) override
    {
        _filepath = path;
    }

    QList<const IFileModel<> *> children() const override
    {
        return _children;
    }
    void setChildren(const QList<const IFileModel<> *> &children) override
    {
        _children = children;
    }

    IFileModel<> *parent() const override
    {
        return _parent;
    }
    void setParent(IFileModel<> *parent) override
    {
        _parent = parent;
    }
    QFileInfo fileInterface() const  override
    {
        return QFileInfo(filepath());
    }

private:
    QString _filepath;
    IFileModel<> *_parent = nullptr;
    QList<const IFileModel<>*>_children;

};

struct DefaultFileModelArguments
{
    QFileInfo fileInfo;
    QString filepath;
};

typedef QList<const IFileModel<QFileInfo>*> DefaultFileModelList;
#endif // FILEOBJECTMODEL_H






