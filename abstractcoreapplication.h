#ifndef ABSTRACTCOREAPPLICATION_H
#define ABSTRACTCOREAPPLICATION_H

#include <qstring.h>
#include <QTreeWidgetItem>

#include "rules.h"

class AbstractCoreApplication
{
public:
    virtual ~AbstractCoreApplication();

    virtual void clearFolders(QStringList paths) = 0;

    virtual void addWatchFolder(QString path) = 0;
    virtual void addWatchFolders(QStringList paths) = 0;
    virtual QString watchFolder(int index) const = 0;
    virtual QStringList watchFolders() = 0;
    virtual QList<QTreeWidgetItem*> watchFolderItems() = 0;
    virtual void removeWatchFolderAt(int index) = 0;
    virtual void removeWatchFolder(QString path) = 0;

    virtual QString directoryInformationHtml(QString path) = 0;

    virtual void insertRule(Rule r) = 0;
    virtual void replaceRule(Rule newRule, int index) = 0;
    virtual void replaceRule(Rule newRule, QString title) = 0;
    virtual QList<QTreeWidgetItem*> ruleItems() = 0;
    virtual void removeRuleAt(int index) = 0;
    virtual void removeRule(QString title) = 0;
    virtual Rule ruleAt(int index) = 0;
    virtual Rule rule(QString title) = 0;

};

#endif // ABSTRACTCOREAPPLICATION_H
