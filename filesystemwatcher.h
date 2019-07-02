#ifndef FILESYSTEMWATCHER_H
#define FILESYSTEMWATCHER_H

#include <qfilesystemwatcher.h>
#include <entitymodel.h>

class FileSystemWatcher : public QFileSystemWatcher
{
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
