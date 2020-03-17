#ifndef ABSTRACTRULESMANAGER_H
#define ABSTRACTRULESMANAGER_H

#include <QTreeWidgetItem>
#include "rules.h"
#include "imutableobject.h"
#include "ifilelistservice.h"
#include "filemodeldelegate.h"

class AbstractRulesManager :
        public QObject,
        public IMutableObject
{
    Q_OBJECT
public:

    virtual FileModelList filterAccordingToCriterias(const FileModelList &list,const Rule &rule,IFileListService *listService) = 0;

    // State alteration methods
    virtual void swapRule(int i, int j) = 0;
    virtual void addRule(const Rule &r) = 0;
    virtual void addRules(const QList<Rule> &r) = 0;
    virtual void replaceRule(const Rule &r, int index) = 0;
    virtual void replaceRule(const Rule &r, QString title) = 0;
    virtual void removeRuleAt(const int &i) = 0;
    virtual void removeRule(const QString &title) = 0;

    // Get
    virtual QList<Rule> rules() const = 0;
    virtual Rule rule(int index) const  = 0;
    virtual const Rule rule(const QString &title) const = 0;
    virtual int rulesCount() const = 0;

    // Get generic models
    virtual QList<QTreeWidgetItem*>ruleItems() const = 0;
signals:
    // IMutableObject interface
    void stateChanged();
};

#endif // ABSTRACTRULESMANAGER_H
