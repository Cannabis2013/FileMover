#ifndef ABSTRACTCOREAPPLICATION_H
#define ABSTRACTCOREAPPLICATION_H

#include <qstring.h>
#include "fileoperationworker.h"
#include "myicon.h"
#include "rulesmanager.h"
#include "settingsdelegate.h"
#include "mutableobject.h"

class ICoreApplication : public MutableObject
{
    Q_OBJECT
public:
    virtual ~ICoreApplication();

    virtual QString watchFolder(int index) const = 0;
    virtual void clearWatchFolders() = 0;
    virtual QStringList watchFolders() = 0;
    virtual QList<QTreeWidgetItem*> watchFolderItems() = 0;
    virtual QList<QTreeWidgetItem*> detailedWatchFolderItems() = 0;

    virtual Rule ruleAt(int index) = 0;
    virtual Rule rule(QString title) = 0;
    virtual QList<QTreeWidgetItem*> ruleItemModels() = 0;
    virtual void swapRule(int i, int j) = 0;
    virtual void clearRules() const = 0;

    virtual QString directoryInformationHtml(QString path) = 0;

    virtual QList<QTreeWidgetItem*> suffixList(QString path) = 0;

    virtual QList<MyIcon> icons() = 0;

    virtual SettingsDelegate settingsState() = 0;
    virtual void setSettings(SettingsDelegate s) =0;

    virtual bool closeOnExit() = 0;

    virtual void calculateFolderSize(QString path) = 0;

public slots:
    virtual void clearFolders(QStringList paths) = 0;
    virtual void clearFoldersAccordingToRules(QStringList paths) = 0;

    virtual void addWatchFolder(QString path) = 0;
    virtual void addWatchFolders(QStringList paths) = 0;

    virtual void removeWatchFolderAt(int index) = 0;
    virtual void removeWatchFolder(QString path) = 0;

    virtual void insertRule(Rule r) = 0;
    virtual void replaceRule(Rule newRule, QString title) = 0;
    virtual void removeRuleAt(int index) = 0;
    virtual void removeRule(QString title) = 0;

signals:
    // FileWorker

    void sendFolderSize(DirectoryCountEntity *fObject);
    void sendStatusMessage(const QString &filePath);
    void sendEntity(EntityModel *model);

signals:

};

#endif // ABSTRACTCOREAPPLICATION_H
