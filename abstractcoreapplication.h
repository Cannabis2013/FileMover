#ifndef ABSTRACTCOREAPPLICATION_H
#define ABSTRACTCOREAPPLICATION_H

#include <qstring.h>

#include "myIcon.h"
#include "rulesmanager.h"

class AbstractCoreApplication : public QObject
{
    Q_OBJECT
public:
    virtual ~AbstractCoreApplication();

    virtual QString watchFolder(int index) const = 0;
    virtual QStringList watchFolders() = 0;
    virtual QList<QTreeWidgetItem*> watchFolderItems() = 0;
    virtual QList<QTreeWidgetItem*> detailedWatchFolderItems() = 0;

    virtual Rule ruleAt(int index) = 0;
    virtual Rule rule(QString title) = 0;
    virtual QList<QTreeWidgetItem*> ruleItems() = 0;
    virtual void swapRule(int i, int j) = 0;

    virtual QString directoryInformationHtml(QString path) = 0;

    virtual QList<QTreeWidgetItem*> suffixList(QString path) = 0;

    virtual QList<MyIcon> icons() = 0;

    virtual bool closeOnExit() = 0;

    virtual void readPersistence() = 0;

public slots:
    virtual void clearFolders(QStringList paths) = 0;

    virtual void addWatchFolder(QString path) = 0;
    virtual void addWatchFolders(QStringList paths) = 0;

    virtual void removeWatchFolderAt(int index) = 0;
    virtual void removeWatchFolder(QString path) = 0;

    virtual void insertRule(Rule r) = 0;
    virtual void replaceRule(Rule newRule, QString title) = 0;
    virtual void removeRuleAt(int index) = 0;
    virtual void removeRule(QString title) = 0;


signals:

    // Notify observers

    void stateChanged();

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

};

#endif // ABSTRACTCOREAPPLICATION_H
