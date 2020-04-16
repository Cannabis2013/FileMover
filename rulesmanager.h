#ifndef RULECONTROLLER_H
#define RULECONTROLLER_H

#include <QListWidgetItem>
#include <qapplication.h>

#include "abstractpersistence.h"
#include "abstractrulesmanager.h"
#include "ifilelistservice.h"
#include "defaultRuleConfiguration.h"
#include "rulescontext.h"

class rulesManager :
        public AbstractRulesManager,
        private AbstractPersistence
{
public:
    rulesManager(const QString &appName, const QString &orgName, IDefaultRuleBuilder *ruleBuilderService);
    ~rulesManager();

    // Persistence
    void readSettings() override;
    void writeSettings() override;

    // State manipulation
    void swapRule(int i, int j) override;
    void addRule(const IRule<DefaultRuleCriteria>* r) override;
    void addRules(const QList<const IRule<DefaultRuleCriteria>*> &r) override;
    void replaceRule(const IRule<DefaultRuleCriteria>* r, int index) override;
    void replaceRule(const IRule<DefaultRuleCriteria>* r, QString title) override;
    void removeRuleAt(const int &i) override {_rules.removeAt(i);}
    void removeRule(const QString &title) override;
    // Get
    QList<const DefaultRuleInterface *> rules() const override;
    const DefaultRuleInterface *rule(int index)  const override {return _rules.value(index);}
    const DefaultRuleInterface *rule(const QString &title) const override;
    int rulesCount() const override {return _rules.count();}

    // Get generic models
    QList<QTreeWidgetItem*>ruleItems() const override;

private:
    QList<const IRule<DefaultRuleCriteria>*> _rules;
};

#endif // RULECONTROLLER_H
