#ifndef FILESYSTEMWATCHER_H
#define FILESYSTEMWATCHER_H

#include <qfilesystemwatcher.h>
#include "fileoperationworker.h"

class FileSystemWatcher :
        public QObject,
        public MutableObject,
        public BroadcastingObject
{
    Q_OBJECT
public:
    FileSystemWatcher(const QStringList &paths);
    void removePath(const QString &path);

signals:
    void folderChanged(EntityModel *eModel);
    // Reimplemented from BroadcastingObject
    void sendSystemTrayMessage(const QString &title, const QString &msg);
    void sendStatusLineMessage(const QString &msg);
    // Reimplemented from MutableObject
    void stateChanged();

private slots:

    void changed(const QString &path);

private:
    QFileSystemWatcher *fWatcher;

};

#endif // FILESYSTEMWATCHER_H
