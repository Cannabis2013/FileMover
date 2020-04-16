#ifndef IFILEMODEL_H
#define IFILEMODEL_H

#include "QFileInfo"
#include "imodel.h"

template<class TFileInterface = QFileInfo, typename TUniqueIdentifier = int>
class IFileModel : public IModel<int,TUniqueIdentifier>
{
public:
    virtual QString filepath() const = 0;
    virtual void setFilepath(const QString &path) = 0;
    virtual const QList<const IFileModel*> children() const = 0;
    virtual void setChildren(const QList<const IFileModel*> &children) = 0;
    virtual IFileModel *parent() const = 0;
    virtual void setParent(IFileModel *parent) = 0;
    virtual TFileInterface fileInterface() const = 0;
};

struct DefaultFileModelArguments
{
    QFileInfo fileInfo;
    QString filepath;
};

#endif // IFILEMODEL_H
