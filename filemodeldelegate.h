#ifndef FILEOBJECTMODEL_H
#define FILEOBJECTMODEL_H

#include <QFileInfo>
#include <QString>
#include <QList>

template<class T> class IFileModel
{
public:
    virtual const IFileModel<T> *parent() const = 0;
    virtual void setParent(const IFileModel<T> *parent) = 0;

    virtual QList<const T *> children() const = 0;
    virtual void setChildren(const QList<T *> &children) = 0;
};

template<class T> class IFileModelDelegate
{
public:
    virtual const T *parentModel() const =  0;
    virtual void setParentModel(const IFileModelDelegate<T> *newParent) = 0;
    virtual const QList<const IFileModelDelegate<T> *> children() const = 0;
    virtual void setChildren(const QList<const IFileModelDelegate<T> *> &value) = 0;
    virtual void appendChild(const IFileModelDelegate<T> *object) = 0;
    virtual bool hasChildren() = 0;
    virtual QString parentFolderName() const = 0;
    virtual const T *model() const = 0;
    virtual T* M() const = 0;

};

#endif // FILEOBJECTMODEL_H
