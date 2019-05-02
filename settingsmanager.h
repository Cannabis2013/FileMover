#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QTimer>
#include <qstring.h>
#include <QTreeWidgetItem>
#include "qlist.h"
#include "abstractpersistence.h"
#include "iconscanner.h"


class settingsManager : public QObject,
        private IconScanner,
        public AbstractPersistence
{
    Q_OBJECT
public:
    settingsManager(QString appName, QString orgName);
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

    // Path Related..

    void insertPath(QString path);
    void insertPaths(QStringList paths);

    void removePath(QString path);
    void removePathAt(int index);

    QStringList paths() {return mainFolderPaths;}
    QList<QTreeWidgetItem*> pathItems();

    bool closeOnQuit() {return closeOnExit;}
    bool isRulesEnabled() {return rulesEnabled;}
    bool countTimerEnabled() {return timerEnabled;}
    QString countTimerInterval() {return QString::number(timerMsec);}

    // Icons related..
    QList<MyIcon>allIcons() const {return trayIconList;}

signals:
    void stateChanged();
    void processPath(QString path);
    void processPaths(QStringList paths);

    void removeItem(QString path);

private:

    QStringList mainFolderPaths;
    QString ressourceFolder = "Ressources";
    QString fileIconPath = "fileIcons";
    int timerMsec;
    bool closeOnExit = false, rulesEnabled = false, timerEnabled = false;
    QTimer countTimer;
    QList<MyIcon> trayIconList;
    QList<MyIcon> fileIconList;

    QIcon currentTrayIcon;
    QIcon fileIconStandard;
};

typedef settingsManager sM;

#endif // SETTINGSCONTROLLER_H
