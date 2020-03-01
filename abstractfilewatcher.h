#ifndef ABSTRACTFILEWATCHER_H
#define ABSTRACTFILEWATCHER_H

#include <qfilesystemwatcher.h>
#include "entitymodel.h"
#include "imutableobject.h"

class AbstractFileWatcher :
        public QObject,
        public IMutableObject,
        public IBroadcastingObject
{
    Q_OBJECT
public:
    virtual void addPath(const QString &path) = 0;
    virtual void removePath(const QString &path) = 0;

signals:
    void folderChanged(EntityModelDelegate<EntityModel> *eModel);

    void sendSystemTrayMessage(const QString &title, const QString &msg);
    void sendStatusLineMessage(const QString &msg);

    void stateChanged();
};

#endif // ABSTRACTFILEWATCHER_H
