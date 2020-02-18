#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

//#define TEST_MODE

#include <qthread.h>
#include "settingsmanager.h"
#include "fileinformationmanager.h"
#include "abstractcoreapplication.h"
#include "filesystemwatcher.h"
#include "threadsmanager.h"


class MainApplication : public AbstractCoreApplication
{
    Q_OBJECT
public:
    MainApplication(const QString &appName,
                    const QString &orgName);

    ~MainApplication();

    QString directoryInformationHtml(QString path){return fManager->createTextBrowserHtml(path);}

    void addWatchFolders(QStringList paths);
    void clearWatchFolders();
    QString watchFolder(int index) const;
    QStringList watchFolders();
    QList<QTreeWidgetItem *> watchFolderItems(){return sManager->pathItems();}
    QList<QTreeWidgetItem*> detailedWatchFolderItems(){return fManager->allTreeItems();}
    int watchFolderCount();

    QList<QTreeWidgetItem*> suffixList(QString path){return fManager->item(path).suffixItems();}

    /*
     * Rules related
     */

    Rule ruleAt(int index) {return rManager->rule(index);}
    Rule rule(QString title){return rManager->rule(title);}
    QList<QTreeWidgetItem*> ruleItemModels(){return rManager->ruleItems();}
    void swapRule(int i, int j){rManager->swapRule(i,j);}
    void clearRules() const;

    QList<MyIcon> icons(){return sManager->allIcons();}

    // Basic settings and persistence related

    SettingsDelegate settingsState();
    void setSettings(SettingsDelegate s);

    bool closeOnExit(){return sManager->closeOnQuit();}

    // File/folder operations

    void calculateFolderSize(QString path);

    void clearFolders(QStringList paths);
    void clearFoldersAccordingToRules(QStringList paths);


public slots:
    void addWatchFolder(QString path){sManager->insertPath(path);}
    void removeWatchFolderAt(int index);
    void removeWatchFolder(QString path);

    void insertRule(Rule r){rManager->addRule(r);}
    void replaceRule(Rule newRule, QString title){rManager->replaceRule(newRule,title);}
    void removeRuleAt(int index){rManager->removeRuleAt(index);}
    void removeRule(QString title){rManager->removeRule(title);}

private:
    EntityQueueManager *entityManager;
    FileOperationWorker *fWorker;
    rulesManager *rManager;
    FileInformationManager *fManager;
    FileSystemWatcher *fWatcher;
    settingsManager *sManager;
    ThreadsManager *tManager;
    QThread *fileWorkerThread, *queueThread;

};

#endif // MAINAPPLICATION_H
