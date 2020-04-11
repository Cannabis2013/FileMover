#ifndef ABSTRACTFILEWORKER_H
#define ABSTRACTFILEWORKER_H

#include "exceptionhandler.h"
#include "staticviewhelpers.h"
#include "entitymodel.h"
#include "directoryitem.h"
#include "ibroadcastingobject.h"

#include "imodeldelegates.h"

namespace FilesContext
{
    enum copyMode{move = 0,copy = 1,noMode = 2};
    enum FileAction {Move = 0x024, Delete = 0x025, Copy = 0x026, None = 0x01};
}

class AbstractFileWorker :
        public QObject,
        public IBroadcastingObject
{
    Q_OBJECT
public slots:
    // wake-up/Start entity file operations
    virtual void handleProcessRequest() = 0;
    virtual void processEntity(IModelDelegate<EntityModel, DefaultEntityType> *delegate) = 0;

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
