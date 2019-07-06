#include "fileobject.h"

FileObject::FileObject(const QString &filePath):
    QFileInfo(filePath)
{
}

FileObject *FileObject::getParent() const
{
    return parent;
}

void FileObject::setParent(FileObject * const &newParent)
{
    parent = newParent;
}

QList<FileObject *> FileObject::getChildren() const
{
    return children;
}

void FileObject::setChildren(const QList<FileObject *> &value)
{
    children = value;
}

void FileObject::appendChild(FileObject *const &object)
{
    object->setParent(this);
    children.append(object);
}
