#ifndef FILESYSTEMWATCHER_H
#define FILESYSTEMWATCHER_H

#include <qfilesystemwatcher.h>
#include "fileoperationworker.h"

class FileSystemWatcher :
        public QObject,
        public IMutableObject,
        public IBroadcastingObject
{
    Q_OBJECT
public:
    FileSystemWatcher(const QStringList &paths);
    void addPath(const QString &path)
    {
        fWatcher->addPath(path);
    }
    void removePath(const QString &path);

signals:
    void folderChanged(EntityModelDelegate<EntityModel> *eModel);
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
