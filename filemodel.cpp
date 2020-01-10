#include "filemodel.h"

FileModel::FileModel(const QString &filePath):
    QFileInfo(filePath)
{
}

FileModel *FileModel::parentFolderObject() const
{
    return _parent;
}

void FileModel::setParentFolderObject(FileModel * const &newParent)
{
    _parent = newParent;
}


QList<FileModel *> FileModel::children() const
{
    return _children;
}

void FileModel::setChildren(const QList<FileModel *> &value)
{

    _children = value;
}

void FileModel::appendChild(FileModel *const &object)
{
    object->setParentFolderObject(this);
    _children.append(object);
}

const QString FileModel::parentFolderName()
{
    if(_parent != nullptr)
        return _parent->fileName();
    else
        return QString();
}
