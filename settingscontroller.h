#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QTimer>
#include <qicon.h>
#include <qstring.h>
#include "rulecontroller.h"
#include "qlist.h"
#include "myIcon.h"
#include "pathcontroller.h"

/*
 * Basic settings
 * Contain and handle paths
*/

struct settingsContainer
{
    int timerMsec;
    bool closeOnExit, rulesEnabled, timerEnabled;
    QTimer countTimer;
    QList<myIcon>trayIconList;
};

class settingsController : public QObject
{
    Q_OBJECT
public:
    settingsController();

    // void members..
    void setCloseOnExit(bool enable){settings.closeOnExit = enable;}
    void setRulesEnabled(bool enable){settings.rulesEnabled = enable;}
    void setTimerEnabled(bool enable){settings.timerEnabled = enable;}
    void setTimerInterval(int msec){settings.timerMsec = msec;}

    // Icons related..
    void insertIcon(const myIcon ic){settings.trayIconList << ic;}
    void insertIcons(QList<myIcon>icons){settings.trayIconList << icons;}

    // Path Related..

    void insertPath(QString path);
    void insertPaths(QStringList paths);

    // non-void members..
    bool closeOnQuit() const {return settings.closeOnExit;}
    bool isRulesEnabled() const {return settings.rulesEnabled;}
    bool countTimerEnabled() const {return settings.timerEnabled;}
    QString countTimerInterval() const {return QString::number(settings.timerMsec);}

    // Icons related..
    QList<myIcon>trayIcons() const {return settings.trayIconList;}

signals:
    void sendPathToMainWindow(QString path);
    void sendPathsToMainWindow(QStringList paths);
    void processPath(QString path);
    void processPaths(QStringList paths);

private:

    // Non-void Methods..

    QStringList Paths() const{return pControl->Paths();}

    pathController *pControl;
    settingsContainer settings;

    friend class settingsWindow;
};

typedef settingsController sC;

#endif // SETTINGSCONTROLLER_H
