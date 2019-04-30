#ifndef RULECONTROLLER_H
#define RULECONTROLLER_H

#include <QTreeWidgetItem>


#include "abstractpersistence.h"
#include "rules.h"


class rulesManager : private AbstractPersistence
{
public:
    rulesManager(QString appName, QString orgName);

    static QString mergeStringList(const QStringList strings);
    static QStringList splitString(const QString split);

    QList<QTreeWidgetItem*>rulesTreeItems() const;

protected:

    void readSettings();
    void writeSettings();

private:
    void writeRulesToReg();

    QList<Rule> ruleslist(){return rules;}

    QList<Rule> rules;

    void addRule(const Rule r);
    void addRules(const QList<Rule>r);
    void replaceRule(const Rule r, int index);
    void replaceRule(const Rule r, QString title);
    void removeRuleAt(const int i){rules.removeAt(i);}
    void removeRule(const QString title);
    Rule rule(int index) {return rules.value(index);}
    Rule rule(QString title);

    friend class MainApplication;
};

#endif // RULECONTROLLER_H
