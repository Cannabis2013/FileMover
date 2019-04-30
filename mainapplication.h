#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

#include <settingsmanager.h>
#include <rulesmanager.h>
#include <qsettings.h>
#include "fileworker.h"
#include "fileinformationmanager.h"
#include "abstractcoreapplication.h"

class MainApplication : public QObject,
        public AbstractPersistence,
        public AbstractCoreApplication,
        private IconScanner
{
    Q_OBJECT
public:
    MainApplication(QString appName, QString orgName);

    ~MainApplication();

    void readSettings();
    void writeSettings();

    void clearFolders(QStringList paths);

    void addWatchFolder(QString path);
    void addWatchFolders(QStringList paths);
    QString watchFolder(int index) const;
    QStringList watchFolders();
    QList<QTreeWidgetItem *> watchFolderItems(){return sManager->pathItems();}
    void removeWatchFolderAt(int index);
    void removeWatchFolder(QString path);

    QString directoryInformationHtml(QString path){return fManager->createTextBrowserHtml(path);}

    void insertRule(Rule r){rManager->addRule(r);}
    void replaceRule(Rule newRule, int index){rManager->replaceRule(newRule,index);}
    void replaceRule(Rule newRule, QString title){rManager->replaceRule(newRule,title);}
    QList<QTreeWidgetItem*> ruleItems(){return rManager->rulesTreeItems();}
    void removeRuleAt(int index){rManager->removeRuleAt(index);}
    void removeRule(QString title){rManager->removeRule(title);}
    Rule ruleAt(int index) {return rManager->rule(index);}
    Rule rule(QString title){return rManager->rule(title);}

signals:

    // fileInformation..

    void processDirectory(DirectoryItem item);
    void processDirectories(QStringList paths);

    /*
     * DetailedFolderView related..
     * Connected to "FileWorker::processFileInformation/FileWorker::processFileInformations"..
     * Is then returned to "mainWindow::recieveDirectoryItem/mainWindow::recieveDirectoryItems"..
     */

    void processPath(QString path);
    void processPaths(QStringList paths);

private slots:
    void recieveDirectoryItem(DirectoryItem item);
    void recieveDirectoryItems(QList<DirectoryItem>items);

private:

    FileWorker *fWorker;
    ProcessManager *pManager;
    rulesManager *rManager;
    settingsManager *sManager;
    FileInformationManager *fManager;

};

#endif // MAINAPPLICATION_H
