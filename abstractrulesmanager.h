#ifndef ABSTRACTRULESMANAGER_H
#define ABSTRACTRULESMANAGER_H

#include <QTreeWidgetItem>

#include "iruleinterfaces.h"

#include "imutableobject.h"

#include "defaultinjector.h"

class AbstractRulesManager :
        public QObject,
        public IMutableObject,
        public DefaultServiceInjector
{
    Q_OBJECT
public:

    // State alteration methods
    virtual void swapRule(int i, int j) = 0;
    virtual void addRule(const DefaultRuleInterface *r) = 0;
    virtual void addRules(const QList<const DefaultRuleInterface*> &r) = 0;
    virtual void replaceRule(const DefaultRuleInterface *r, int index) = 0;
    virtual void replaceRule(const DefaultRuleInterface *r, QString title) = 0;
    virtual void removeRuleAt(const int &i) = 0;
    virtual void removeRule(const QString &title) = 0;

    // Get
    virtual QList<const DefaultRuleInterface*> rules() const = 0;
    virtual const DefaultRuleInterface *rule(int index) const  = 0;
    virtual const DefaultRuleInterface *rule(const QString &title) const = 0;
    virtual int rulesCount() const = 0;

    // Get generic models
    virtual QList<QTreeWidgetItem*>ruleItems() const = 0;


signals:
    // IMutableObject interface
    void stateChanged();
};

#endif // ABSTRACTRULESMANAGER_H
