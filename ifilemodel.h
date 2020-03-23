#ifndef IFILEMODEL_H
#define IFILEMODEL_H

#include "QFileInfo"

template<class TFileInterface = QFileInfo>
class IFileModel
{
public:
    virtual QString filepath() const = 0;
    virtual void setFilepath(const QString &path) = 0;
    virtual QList<const IFileModel*> children() const = 0;
    virtual void setChildren(const QList<const IFileModel*> &children) = 0;
    virtual IFileModel *parent() const = 0;
    virtual void setParent(IFileModel *parent) = 0;
    virtual TFileInterface fileInterface() const = 0;
};

#endif // IFILEMODEL_H
