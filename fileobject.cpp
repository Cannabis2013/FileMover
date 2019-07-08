#include "fileobject.h"

FileObject::FileObject(const QString &filePath):
    QFileInfo(filePath)
{
}

FileObject *FileObject::parentFolderObject() const
{
    return parent;
}

void FileObject::setParentFolderObject(FileObject * const &newParent)
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
    object->setParentFolderObject(this);
    children.append(object);
}

const QString FileObject::parentFolderName()
{
    if(parent != nullptr)
        return parent->fileName();
    else
        return QString();
}
