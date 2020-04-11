#ifndef ABSTRACTFILEWATCHER_H
#define ABSTRACTFILEWATCHER_H

#include <qfilesystemwatcher.h>
#include "entitymodel.h"
#include "imutableobject.h"
#include "ibroadcastingobject.h"
#include "isettingsmodel.h"

class AbstractFileSystemWatcher :
        public QObject,
        public IMutableObject,
        public IBroadcastingObject
{
    Q_OBJECT
public:
    virtual void addPath(const QString &path) = 0;
    virtual void addPaths(const QStringList &paths) = 0;
    virtual void removePath(const QString &path) = 0;



signals:
    void folderChanged(DefaultDelegate *delegate);

    void sendSystemTrayMessage(const QString &title, const QString &msg);
    void sendStatusLineMessage(const QString &msg);

    void stateChanged();
};

#endif // ABSTRACTFILEWATCHER_H
