#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QTimer>
#include <qstring.h>
#include <QTreeWidgetItem>
#include "qlist.h"
#include "abstractpersistence.h"
#include "icon.h"
#include <qdir.h>
#include <qdiriterator.h>
#include "isettingsdelegate.h"
#include "entitymodel.h"
#include "imutableobject.h"
#include <qdatastream.h>
#include <settingsdelegate.h>

class ISettingsManagerInterface : public IMutableObject
{
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

    virtual void processPath(IModelDelegate<EntityModel,EntityType> *delegate) = 0;
    virtual void removeItem(QString path) = 0;
    virtual void stateChanged() = 0;
};

class settingsManager : public ISettingsManagerInterface,
        public QObject,
        private AbstractPersistence
{
    Q_OBJECT
public:
    settingsManager(const QString &appName, const QString &orgName);
    ~settingsManager();

    // Persistence
    void readSettings();
    void writeSettings();

    // Basic settings
    void setCloseOnExit(bool enable);
    void setRulesEnabled(bool enable);
    void setTimerEnabled(bool enable);
    void setTimerInterval(int msec);

    // Icons
    void insertIcon(const AbstractIcon *ic){trayIconList << ic;}
    void insertIcons(const QList<const AbstractIcon*> &icons){trayIconList << icons;}

    const ISettingsDelegate* settingsState() const;
    void setSettings(const ISettingsDelegate *s);

    // Path Related..

    int folderCount() const {return watchFolders.count();}

    void insertPath(QString path);
    void insertPath(const QStringList& paths);

    void removePath(QString path);
    void removePathAt(int index);
    void clearPaths();

    // Request file object processing

    void requestProcess();

    QStringList paths() const {return watchFolders;}
    QList<QTreeWidgetItem*> pathItems() const;

    bool closeOnQuit() const {return _settings->closeOnExit();}
    bool isRulesEnabled() const {return _settings->rulesEnabled();}
    bool countTimerEnabled() const {return _settings->ruleTimerEnabled();}
    QString countTimerInterval() const {return QString::number(_settings->ruleCountInterval());}

    // Icons related..
    QList<const AbstractIcon*> allIcons() const {return trayIconList;}

signals:
    void processPath(IModelDelegate<EntityModel,EntityType> *delegate);
    void removeItem(QString path);
    void stateChanged();

private:
    QList<const AbstractIcon *> scanForIcons(QString path);
    QStringList watchFolders;
    QString ressourceFolder = "Ressources";
    QString fileIconPath = "fileIcons";
    const ISettingsDelegate* _settings;
    QTimer countTimer;
    QList<const AbstractIcon*> trayIconList;
    QList<const AbstractIcon*> fileIconList;
    const AbstractIcon *currentTrayIcon;
    const AbstractIcon * fileIconStandard;

};

typedef settingsManager sM;

#endif // SETTINGSCONTROLLER_H
