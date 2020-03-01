#ifndef ABSTRACTFILEWORKER_H
#define ABSTRACTFILEWORKER_H

#include "entityqueuemanager.h"
#include "exceptionhandler.h"
#include "staticviewhelpers.h"

class AbstractFileWorker :
        public QObject,
        public IBroadcastingObject
{
    Q_OBJECT
public:

    virtual FileObjectList processFileObjects(const FileObjectList &fileObjects,const SubRule &rule) = 0;

    virtual FileObjectList generateFileObjects(const QStringList &paths,
                                              const QString &rPath = QString(),
                                              const RRT::FileTypeEntity &filter = RRT::File) = 0;
public slots:

    virtual void countFolderItems(const QString &path,
                    const QDir::Filters &filters = QDir::NoFilter,
                    const QDirIterator::IteratorFlags &flags = QDirIterator::NoIteratorFlags) = 0;
    virtual void countFolders(const QStringList &Paths) = 0;

    // wake-up/Start entity file operations
    virtual void handleProcessRequest() = 0;
    virtual void processEntity(IModelDelegate<EntityModel, EntityType> *delegate) = 0;

signals:
    void fileCount(long antal);
    void sendFolderSizeEntity(const DirectoryEntity *fObj);
    void sendFolderSizeEntities(QList<DirectoryEntity> s);

    void clearFinished(bool a);

    // Queue related..
    void requestNextEntity();
    void infoReport(const QString &error);
    void processFinished(QList<DirectoryItem>items);
    void jobDone(bool status);

    void sendSystemTrayMessage(const QString &title, const QString &msg);
    void sendStatusLineMessage(const QString &msg);
};

#endif // ABSTRACTFILEWORKER_H
