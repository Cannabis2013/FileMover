#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

//#define TEST_MODE

#include <qthread.h>
#include "settingsmanager.h"
#include "fileinformationmanager.h"
#include "abstractapplicationservice.h"
#include "filesystemwatcher.h"
#include "threadsmanager.h"


class ApplicationDomain : public AbstractApplicationService
{
public:
    ApplicationDomain();

    ~ApplicationDomain();

    QString directoryInformationHtml(QString path) override {return fileManagerService->createTextBrowserHtml(path);}


    // Watchfolder and detailed folder interface
    void addWatchFolders(QStringList paths) override;
    void clearWatchFolders() override;
    QString watchFolder(int index) const override;
    QStringList watchFolders() override;
    QList<QTreeWidgetItem *> watchFolderItems() override {return settingsManagerService->pathItems();}
    QList<QTreeWidgetItem*> detailedWatchFolderItems() override {return fileManagerService->allTreeItems();}
    int watchFolderCount() override;

    QList<QTreeWidgetItem*> suffixList(const QString &path) override {return fileManagerService->suffixList(path);}

    /*
     * Rules interface
     */

    Rule ruleAt(int index) override {return ruleManagerService->rule(index);}
    Rule rule(QString title) override {return ruleManagerService->rule(title);}
    QList<QTreeWidgetItem*> ruleItemModels() override {return ruleManagerService->ruleItems();}
    void swapRule(int i, int j) override {ruleManagerService->swapRule(i,j);}
    void clearRules() const override ;

    void insertRule(Rule r) override {ruleManagerService->addRule(r);}
    void replaceRule(Rule newRule, QString title) override {ruleManagerService->replaceRule(newRule,title);}
    void removeRuleAt(int index) override {ruleManagerService->removeRuleAt(index);}
    void removeRule(QString title) override {ruleManagerService->removeRule(title);}


    // Persistence interface

    const ISettingsDelegate* settingsState() override;
    void setSettings(const ISettingsDelegate *s) override;

    // Basic settings interface
    bool closeOnExit() override {return settingsManagerService->closeOnQuit();}
    QList<const AbstractIcon*> icons() override {return settingsManagerService->allIcons();}

    // File/folder operations

    void calculateFolderSize(QString path)  override;

    void clearFolders(QStringList paths)  override;
    void clearFoldersAccordingToRules(QStringList paths)  override;

    void configureServices()  override;
    void startServices() override;

    void setRuleManagerService(AbstractRulesManager *service) override {ruleManagerService = service;}
    void setSettingsManagerService(AbstractSettingsManager *service) override {settingsManagerService = service;};
    void setEntityQueueManagerService(AbstractQueueManager* service) override {entityQueueManagerService = service;}
    void setFileInformationManagerService(AbstractFileInformationManager *service) override {fileManagerService = service;}
    void setThreadManagerService(IThreadManagerInterface *service) override {threadManagerService = service;}

    void setFileOperationsService(AbstractFileWorker *service) override {fileOperationsService = service;}
    void setFileWatcherService(AbstractFileSystemWatcher *service) override {fileWatcherService = service;}

    void addWatchFolder(QString path) override {settingsManagerService->insertPath(path);}
    void removeWatchFolderAt(int index) override;
    void removeWatchFolder(QString path) override;

private:
    AbstractFileWorker *fileOperationsService;
    AbstractFileSystemWatcher *fileWatcherService;

    // Manager interfaces
    AbstractRulesManager *ruleManagerService;
    AbstractQueueManager *entityQueueManagerService;
    AbstractFileInformationManager *fileManagerService;
    AbstractSettingsManager *settingsManagerService;
    IThreadManagerInterface *threadManagerService;
};

#endif // MAINAPPLICATION_H
