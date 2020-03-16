#ifndef RULECONTROLLER_H
#define RULECONTROLLER_H

#include <QListWidgetItem>

#include <qapplication.h>

#include "abstractpersistence.h"
#include "abstractrulesmanager.h"

#include "ifilelistservice.h"


class rulesManager :
        public AbstractRulesManager,
        private AbstractPersistence
{
public:
    rulesManager(const QString &appName, const QString &orgName);
    ~rulesManager();

    // Persistence
    void readSettings() override;
    void writeSettings() override;

    // State manipulation
    void swapRule(int i, int j) override;
    void addRule(const Rule &r) override;
    void addRules(const QList<Rule> &r) override;
    void replaceRule(const Rule &r, int index) override;
    void replaceRule(const Rule &r, QString title) override;
    void removeRuleAt(const int &i) override {_rules.removeAt(i);}
    void removeRule(const QString &title) override;
    // Get
    QList<Rule> rules() const override;
    Rule rule(int index)  const override {return _rules.value(index);}
    const Rule rule(const QString &title) const override;
    int rulesCount() const override {return _rules.count();}

    // Get generic models
    QList<QTreeWidgetItem*>ruleItems() const override;

private:
    QList<Rule> _rules;
};

#endif // RULECONTROLLER_H
