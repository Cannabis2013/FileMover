#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QTimer>
#include <qstring.h>
#include <QTreeWidgetItem>
#include "qlist.h"
#include "abstractpersistence.h"
#include "myicon.h"
#include <qdir.h>
#include <qdiriterator.h>
#include "settingsdelegate.h"
#include "entitymodel.h"
#include "imutableobject.h"
#include <qdatastream.h>

class settingsManager :
        public QObject,
        public IMutableObject,
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
    void insertIcon(const MyIcon ic){trayIconList << ic;}
    void insertIcons(QList<MyIcon>icons){trayIconList << icons;}

    SettingsDelegate settingsState();
    void setSettings(SettingsDelegate s);

    // Path Related..

    int folderCount(){return watchFolders.count();}

    void insertPath(QString path);
    void insertPath(const QStringList& paths);

    void removePath(QString path);
    void removePathAt(int index);
    void clearPaths();

    // Request file object processing

    void requestProcess();

    QStringList paths() {return watchFolders;}
    QList<QTreeWidgetItem*> pathItems();

    bool closeOnQuit() {return _settings->closeOnExit;}
    bool isRulesEnabled() {return _settings->rulesEnabled;}
    bool countTimerEnabled() {return _settings->ruleTimerEnabled;}
    QString countTimerInterval() {return QString::number(_settings->ruleCountInterval);}

    // Icons related..
    QList<MyIcon>allIcons() const {return trayIconList;}

signals:
    void processPath(EntityModelDelegate *delegate);
    void removeItem(QString path);

    void stateChanged();
private:
    QList<MyIcon> scanForIcons(QString path);
    QStringList watchFolders;
    QString ressourceFolder = "Ressources";
    QString fileIconPath = "fileIcons";
    SettingsDelegate *_settings;
    QTimer countTimer;
    QList<MyIcon> trayIconList;
    QList<MyIcon> fileIconList;
    QIcon currentTrayIcon;
    QIcon fileIconStandard;

};

typedef settingsManager sM;

#endif // SETTINGSCONTROLLER_H
