#ifndef FILESYSTEMWATCHER_H
#define FILESYSTEMWATCHER_H

#include "abstractfilewatcher.h"
class FileSystemWatcher :public AbstractFileWatcher
{
    Q_OBJECT
public:
    FileSystemWatcher();
    void addPath(const QString &path) override
    {
        fWatcher->addPath(path);
    }
    void removePath(const QString &path) override;

private slots:
    void changed(const QString &path);

private:
    QFileSystemWatcher *fWatcher;
};

#endif // FILESYSTEMWATCHER_H
