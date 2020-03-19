#ifndef ABSTRACTRULESMANAGER_H
#define ABSTRACTRULESMANAGER_H

#include <QTreeWidgetItem>

#include "iruleinterfaces.h"

#include "imutableobject.h"
#include "ifilelistservice.h"
#include "filemodeldelegate.h"

#include "defaultinjector.h"

class AbstractRulesManager :
        public QObject,
        public IMutableObject,
        public DefaultServiceInjector
{
    Q_OBJECT
public:

    virtual FileModelList filterAccordingToCriterias(const FileModelList &list,const IRule<IDefaultRuleCondition> *rule,IFileListService *listService) = 0;

    // State alteration methods
    virtual void swapRule(int i, int j) = 0;
    virtual void addRule(const IRule<IDefaultRuleCondition> *r) = 0;
    virtual void addRules(const QList<const IRule<IDefaultRuleCondition>*> &r) = 0;
    virtual void replaceRule(const IRule<IDefaultRuleCondition> *r, int index) = 0;
    virtual void replaceRule(const IRule<IDefaultRuleCondition> *r, QString title) = 0;
    virtual void removeRuleAt(const int &i) = 0;
    virtual void removeRule(const QString &title) = 0;

    // Get
    virtual QList<const IRule<IDefaultRuleCondition>*> rules() const = 0;
    virtual const IRule<IDefaultRuleCondition> *rule(int index) const  = 0;
    virtual const IRule<IDefaultRuleCondition> *rule(const QString &title) const = 0;
    virtual int rulesCount() const = 0;

    // Get generic models
    virtual QList<QTreeWidgetItem*>ruleItems() const = 0;


signals:
    // IMutableObject interface
    void stateChanged();
};

#endif // ABSTRACTRULESMANAGER_H
