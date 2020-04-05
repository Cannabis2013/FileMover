#ifndef IFILEMODEL_H
#define IFILEMODEL_H

#include "QFileInfo"

template<class TFileInterface = QFileInfo>
class IFileModel
{
public:
    virtual QString filepath() const = 0;
    virtual void setFilepath(const QString &path) = 0;
    virtual const QList<const IFileModel<>*> children() const = 0;
    virtual void setChildren(const QList<const IFileModel*> &children) = 0;
    virtual IFileModel *parent() const = 0;
    virtual void setParent(IFileModel *parent) = 0;
    virtual TFileInterface fileInterface() const = 0;
};

typedef QList<const IFileModel<QFileInfo>*> DefaultIFileModelList;

struct DefaultFileModelArguments
{
    QFileInfo fileInfo;
    QString filepath;
};

#endif // IFILEMODEL_H
