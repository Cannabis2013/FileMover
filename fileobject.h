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

    QList<FileObject*> getChildren() const;
    void setChildren(const QList<FileObject *> &value);
    void appendChild(FileObject * const &object);
    bool hasChildren(){return !children.isEmpty();}

    const QString parentFolderName();

    QString getFileName() const;
private:
    FileObject *parent = nullptr;
    QList<FileObject*>children;
};

typedef QList<FileObject*> FileObjectList;
#endif // FILEOBJECTMODEL_H
