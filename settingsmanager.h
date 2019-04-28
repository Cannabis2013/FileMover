#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QTimer>
#include <qicon.h>
#include <qstring.h>
#include "rulesmanager.h"
#include "qlist.h"
#include "MyIcon.h"
#include "abstractpersistence.h"
#include "iconscanner.h"

/*
 * Basic settings
 * Contain and handle paths
*/

struct RessourcePaths
{
    QString ressourceFolder = "Ressources";
    QString fileIconPath = "fileIcons";
};

struct SettingsContainer
{
    int timerMsec;
    bool closeOnExit, rulesEnabled, timerEnabled;
    QTimer countTimer;
    QList<MyIcon> trayIconList;
    QList<MyIcon> fileIconList;

    QIcon currentTrayIcon;
    QIcon fileIconStandard;

    QStringList mainFolderPaths;
};

class settingsManager : public QObject,
        private IconScanner,
        private AbstractPersistence
{
    Q_OBJECT
public:
    settingsManager(QString appName, QString orgName);
    // void members..
    void setCloseOnExit(bool enable){settings.closeOnExit = enable;}
    void setRulesEnabled(bool enable){settings.rulesEnabled = enable;}
    void setTimerEnabled(bool enable){settings.timerEnabled = enable;}
    void setTimerInterval(int msec){settings.timerMsec = msec;}

    // Icons related..
    void insertIcon(const MyIcon ic){settings.trayIconList << ic;}
    void insertIcons(QList<MyIcon>icons){settings.trayIconList << icons;}

    // Path Related..

    void insertPath(QString path);
    void insertPaths(QStringList paths);

    QStringList paths() const {return settings.mainFolderPaths;}

    bool closeOnQuit() const {return settings.closeOnExit;}
    bool isRulesEnabled() const {return settings.rulesEnabled;}
    bool countTimerEnabled() const {return settings.timerEnabled;}
    QString countTimerInterval() const {return QString::number(settings.timerMsec);}

    // Icons related..
    QList<MyIcon>trayIcons() const {return settings.trayIconList;}

signals:
    void stateChanged();

protected:
    void readSettings();
    void writeSettings();
private:

    SettingsContainer settings;
    RessourcePaths rPaths;
};

typedef settingsManager sM;

#endif // SETTINGSCONTROLLER_H
