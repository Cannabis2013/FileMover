#ifndef ABSTRACTSETTINGSMANAGER_H
#define ABSTRACTSETTINGSMANAGER_H

#include <QString>
#include <QList>

#include "imutableobject.h"
#include "abstracticon.h"
#include "isettingsdelegate.h"
#include "entitymodel.h"
#include "QTreeWidgetItem"

class AbstractSettingsManager : public QObject,
        public IMutableObject
{
    Q_OBJECT
public:
    // Basic settings
    virtual void setCloseOnExit(bool enable) = 0;
    virtual void setRulesEnabled(bool enable) = 0;
    virtual void setTimerEnabled(bool enable) = 0;
    virtual void setTimerInterval(int msec) = 0;

    virtual void insertIcon(const AbstractIcon *ic) = 0;
    virtual void insertIcons(const QList<const AbstractIcon*> &icons) = 0;

    virtual const ISettingsDelegate* settingsState() const = 0;
    virtual void setSettings(const ISettingsDelegate *s) = 0;

    // Path Related..

    virtual int folderCount() const = 0;

    virtual void insertPath(QString path) = 0;
    virtual void insertPath(const QStringList& paths) = 0;

    virtual void removePath(QString path) = 0;
    virtual void removePathAt(int index) = 0;
    virtual void clearPaths() = 0;

    // Request file object processing

    void requestProcess() const;

    virtual QStringList paths() const = 0;
    virtual QList<QTreeWidgetItem*> pathItems() const = 0;;

    virtual bool closeOnQuit() const = 0;
    virtual bool isRulesEnabled()  const = 0;
    virtual bool countTimerEnabled() const = 0;
    virtual QString countTimerInterval() const = 0;

    // Icons related..
    virtual QList<const AbstractIcon*>allIcons() const = 0;

    // Virtual signals
signals:
    void processPath(IModelDelegate<EntityModel,EntityType> *delegate);
    void removeItem(QString path);
    void stateChanged();
};

#endif // ABSTRACTSETTINGSMANAGER_H
