#ifndef FILESYSTEMWATCHER_H
#define FILESYSTEMWATCHER_H

#include "abstractfilesystemwatcher.h"
#include "ientitymodelbuilder.h"
#include "entitymodel.h"


class FileSystemWatcher :public AbstractFileSystemWatcher
{
    Q_OBJECT
public:
    FileSystemWatcher();
    void addPath(const QString &path) override
    {
        fileWatcher->addPath(path);
    }
    void addPaths(const QStringList &paths) override
    {
        fileWatcher->addPaths(paths);
    }
    void removePath(const QString &path) override;

private slots:
    void changed(const QString &path);

private:
    QFileSystemWatcher *fileWatcher;

    IEntityModelBuilder<DefaultDelegate> *builderService;
};

#endif // FILESYSTEMWATCHER_H
