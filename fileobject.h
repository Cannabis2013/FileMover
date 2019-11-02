#ifndef FILEOBJECTMODEL_H
#define FILEOBJECTMODEL_H

#include <QFileInfo>
#include <QString>
#include <QList>

class FileObject : public QFileInfo
{
public:
    FileObject(const QString &filePath);

    FileObject *parentFolderObject() const;
    void setParentFolderObject(FileObject *const &newParent);

    QList<FileObject*> children() const;
    void setChildren(const QList<FileObject *> &value);
    void appendChild(FileObject * const &object);
    bool hasChildren(){return !_children.isEmpty();}

    const QString parentFolderName();

private:
    FileObject *_parent = nullptr;
    QList<FileObject*>_children;
};

typedef QList<FileObject*> FileObjectList;
#endif // FILEOBJECTMODEL_H
