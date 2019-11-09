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
#include "myobject.h"
#include <qdatastream.h>
#include <iostream>

using namespace std;

class settingsManager : public MyObject,
        private AbstractPersistence
{
    Q_OBJECT
public:
    settingsManager(const QString &appName, const QString &orgName);
    ~settingsManager();

    // Persistence
    void readSettings();
    void writeSettings();

    // void members..
    void setCloseOnExit(bool enable);
    void setRulesEnabled(bool enable);
    void setTimerEnabled(bool enable);
    void setTimerInterval(int msec);

    // Icons related..
    void insertIcon(const MyIcon ic){trayIconList << ic;}
    void insertIcons(QList<MyIcon>icons){trayIconList << icons;}

    SettingsDelegate settingsState();
    void setSettings(SettingsDelegate s);

    // Path Related..

    void insertPath(QString path);
    void insertPath(QStringList paths);

    void removePath(QString path);
    void removePathAt(int index);

    // Request file object processing

    void requestProcess();

    QStringList paths() {return mainFolderPaths;}
    QList<QTreeWidgetItem*> pathItems();

    bool closeOnQuit() {return _settings->closeOnExit;}
    bool isRulesEnabled() {return _settings->rulesEnabled;}
    bool countTimerEnabled() {return _settings->ruleTimerEnabled;}
    QString countTimerInterval() {return QString::number(_settings->ruleCountInterval);}

    // Icons related..
    QList<MyIcon>allIcons() const {return trayIconList;}

signals:
    void processPath(EntityModel *entity);

    void removeItem(QString path);

private:
    QList<MyIcon> scanForIcons(QString path);
    QStringList mainFolderPaths;
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
