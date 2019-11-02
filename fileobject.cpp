#include "fileobject.h"

FileObject::FileObject(const QString &filePath):
    QFileInfo(filePath)
{
}

FileObject *FileObject::parentFolderObject() const
{
    return _parent;
}

void FileObject::setParentFolderObject(FileObject * const &newParent)
{
    _parent = newParent;
}


QList<FileObject *> FileObject::children() const
{
    return _children;
}

void FileObject::setChildren(const QList<FileObject *> &value)
{

    _children = value;
}

void FileObject::appendChild(FileObject *const &object)
{
    object->setParentFolderObject(this);
    _children.append(object);
}

const QString FileObject::parentFolderName()
{
    if(_parent != nullptr)
        return _parent->fileName();
    else
        return QString();
}
