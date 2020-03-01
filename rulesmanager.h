#ifndef RULECONTROLLER_H
#define RULECONTROLLER_H

#include <QListWidgetItem>

#include <qapplication.h>

#include "abstractpersistence.h"
#include "fileoperationworker.h"


class AbstractRulesManager : public QObject,
                                      public IMutableObject
{
    Q_OBJECT
public:

    // State alteration methods
    virtual void swapRule(int i, int j) = 0;
    virtual void addRule(const Rule r) = 0;
    virtual void addRules(const QList<Rule>r) = 0;
    virtual void replaceRule(const Rule r, int index) = 0;
    virtual void replaceRule(const Rule r, QString title) = 0;
    virtual void removeRuleAt(const int i) = 0;
    virtual void removeRule(const QString title) = 0;

    // Get
    virtual QList<Rule> ruleslist() const = 0;
    virtual Rule rule(int index) const  = 0;
    virtual const Rule rule(QString title) const = 0;
    virtual int ruleCount() const = 0;

    // Get generic models
    virtual QList<QTreeWidgetItem*>ruleItems() const = 0;
signals:
    // IMutableObject interface
    void stateChanged();
};

class rulesManager :public AbstractRulesManager,
        private AbstractPersistence
{
public:
    rulesManager(const QString &appName, const QString &orgName);
    ~rulesManager();

    // Persistence
    void readSettings();
    void writeSettings();

    // State manipulation
    void swapRule(int i, int j);
    void addRule(const Rule r);
    void addRules(const QList<Rule>r);
    void replaceRule(const Rule r, int index);
    void replaceRule(const Rule r, QString title);
    void removeRuleAt(const int i){rules.removeAt(i);}
    void removeRule(const QString title);

    // Get
    QList<Rule> ruleslist() const {return rules;}
    Rule rule(int index)  const {return rules.value(index);}
    const Rule rule(QString title) const;
    int ruleCount() const {return rules.count();}

    // Get generic models
    QList<QTreeWidgetItem*>ruleItems() const;

private:
    QList<Rule> rules;
};

#endif // RULECONTROLLER_H
