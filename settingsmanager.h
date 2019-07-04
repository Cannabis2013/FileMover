#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QTimer>
#include <qstring.h>
#include <QTreeWidgetItem>
#include "qlist.h"
#include "abstractpersistence.h"
#include "myIcon.h"
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
    void setCloseOnExit(bool enable){closeOnExit = enable;}
    void setRulesEnabled(bool enable){rulesEnabled = enable;}
    void setTimerEnabled(bool enable){timerEnabled = enable;}
    void setTimerInterval(int msec){timerMsec = msec;}

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

    bool closeOnQuit() {return closeOnExit;}
    bool isRulesEnabled() {return rulesEnabled;}
    bool countTimerEnabled() {return timerEnabled;}
    QString countTimerInterval() {return QString::number(timerMsec);}

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
    int timerMsec;
    bool closeOnExit = true, rulesEnabled = false, timerEnabled = false;
    QTimer countTimer;
    QList<MyIcon> trayIconList;
    QList<MyIcon> fileIconList;

    QIcon currentTrayIcon;
    QIcon fileIconStandard;
};

typedef settingsManager sM;

#endif // SETTINGSCONTROLLER_H
