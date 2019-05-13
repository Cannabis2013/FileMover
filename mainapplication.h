#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

#include <qthread.h>
#include "fileworker.h"
#include "settingsmanager.h"
#include "fileinformationmanager.h"
#include "abstractcoreapplication.h"

class MainApplication : public AbstractCoreApplication,
        private IconScanner
{
    Q_OBJECT
public:
    MainApplication(QString appName, QString orgName);

    ~MainApplication();

    QString directoryInformationHtml(QString path){return fManager->createTextBrowserHtml(path);}

    void addWatchFolders(QStringList paths);
    QString watchFolder(int index) const;
    QStringList watchFolders();
    QList<QTreeWidgetItem *> watchFolderItems(){return sManager->pathItems();}
    QList<QTreeWidgetItem*> detailedWatchFolderItems(){return fManager->allTreeItems();}


    QList<QTreeWidgetItem*> suffixList(QString path){return fManager->item(path).suffixItems();}

    Rule ruleAt(int index) {return rManager->rule(index);}
    Rule rule(QString title){return rManager->rule(title);}
    QList<QTreeWidgetItem*> ruleItems(){return rManager->ruleItems();}
    void swapRule(int i, int j){rManager->swapRule(i,j);}

    QList<MyIcon> icons(){return sManager->allIcons();}

    bool closeOnExit(){return sManager->closeOnQuit();}

    void readPersistence();

    void beginCalcSize(QString path){fWorker->calcSize(path);}
    void beginCalcSizes(QStringList paths){fWorker->calcSize(paths);}

public slots:

    void clearFolders(QStringList paths);

    void addWatchFolder(QString path){sManager->insertPath(path);}
    void removeWatchFolderAt(int index);
    void removeWatchFolder(QString path);

    void insertRule(Rule r){rManager->addRule(r);}
    void replaceRule(Rule newRule, QString title){rManager->replaceRule(newRule,title);}
    void removeRuleAt(int index){rManager->removeRuleAt(index);}
    void removeRule(QString title){rManager->removeRule(title);}
private:

    FileWorker *fWorker;
    ProcessManager *pManager;
    rulesManager *rManager;
    settingsManager *sManager;
    FileInformationManager *fManager;
    QThread *fileWorkerThread;
};

#endif // MAINAPPLICATION_H
