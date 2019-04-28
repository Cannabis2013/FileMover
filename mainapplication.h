#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

#include <settingsmanager.h>
#include <rulesmanager.h>
#include <qsettings.h>
#include "abstractpersistence.h"
#include "iconscanner.h"
#include "fileworker.h"

class MainApplication : private IconScanner, private AbstractPersistence
{
public:
    MainApplication(QString appName, QString orgName);
    virtual ~MainApplication();

    void readSettings();
    void writeSettings();

    void clearFolders(QStringList paths);

    void addWatchFolder(QString path);
    QString watchFolder(int index);
    QStringList watchFolders();

private:

    bool closeOnBut;

    fW *fWorker;
    processManager *pManager;
    rM *rManager;
    sM *sManager;
};

#endif // MAINAPPLICATION_H
