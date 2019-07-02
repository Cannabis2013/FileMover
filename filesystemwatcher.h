#ifndef FILESYSTEMWATCHER_H
#define FILESYSTEMWATCHER_H

#include <qfilesystemwatcher.h>
#include "fileoperationsworker.h"

class FileSystemWatcher : public QFileSystemWatcher
{
    Q_OBJECT
public:
    FileSystemWatcher(QStringList paths):
        QFileSystemWatcher(paths)
    {
        connect(this,&FileSystemWatcher::directoryChanged,this,&FileSystemWatcher::_folderChanged);
    }

signals:
    void folderChanged(EntityModel *eModel);
    void notifyChange(QString path);

private slots:

    void _folderChanged(const QString &path)
    {
        emit folderChanged(new fileInformationEntity(path));
        emit notifyChange(path);
    }
};

#endif // FILESYSTEMWATCHER_H
