#ifndef FILEOBJECTMODEL_H
#define FILEOBJECTMODEL_H

#include <QFileInfo>
#include <QString>
#include <QList>

class FileModel : public QFileInfo
{
public:
    FileModel(const QString &filePath);

    FileModel *parentFolderObject() const;
    void setParentFolderObject(FileModel *const &newParent);

    QList<FileModel*> children() const;
    void setChildren(const QList<FileModel *> &value);
    void appendChild(FileModel * const &object);
    bool hasChildren(){return !_children.isEmpty();}

    const QString parentFolderName();

private:
    FileModel *_parent = nullptr;
    QList<FileModel*>_children;
};

typedef QList<FileModel*> FileObjectList;
#endif // FILEOBJECTMODEL_H
