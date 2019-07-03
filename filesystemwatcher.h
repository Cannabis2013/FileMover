#ifndef FILESYSTEMWATCHER_H
#define FILESYSTEMWATCHER_H

#include <qfilesystemwatcher.h>
#include "fileoperationsworker.h"

class FileSystemWatcher : public MyObject
{
    Q_OBJECT
public:
    FileSystemWatcher(QStringList paths):
        fWatcher(new QFileSystemWatcher(paths))
    {
        connect(fWatcher,&QFileSystemWatcher::directoryChanged,this,&FileSystemWatcher::_folderChanged);
    }

    void removePath(const QString &path)
    {
        QStringList paths = fWatcher->directories();
        paths.removeOne(path);
    }

signals:
    void folderChanged(EntityModel *eModel);

private slots:

    void _folderChanged(const QString &path)
    {
        emit folderChanged(new fileInformationEntity(path));
        QString msg = QString("A change in the following folder has occured: \n %1").arg(path);
        emit sendSystemTrayMessage("Information",msg);
    }

private:
    QFileSystemWatcher *fWatcher;
};

#endif // FILESYSTEMWATCHER_H
