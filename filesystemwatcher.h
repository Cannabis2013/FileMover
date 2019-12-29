#ifndef FILESYSTEMWATCHER_H
#define FILESYSTEMWATCHER_H

#include <qfilesystemwatcher.h>
#include "fileoperationworker.h"

class FileSystemWatcher : public MutableObject
{
    Q_OBJECT
public:
    FileSystemWatcher(const QStringList &paths);
    void removePath(const QString &path);

signals:
    void folderChanged(EntityModel *eModel);

private slots:

    void changed(const QString &path);

private:
    QFileSystemWatcher *fWatcher;
};

#endif // FILESYSTEMWATCHER_H
