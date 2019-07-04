#ifndef RULECONTROLLER_H
#define RULECONTROLLER_H

#include <QListWidgetItem>

#include <qapplication.h>

#include "abstractpersistence.h"
#include "rules.h"

class rulesManager : public QObject,
        private AbstractPersistence
{

    Q_OBJECT
public:
    rulesManager(const QString &appName, const QString &orgName);
    ~rulesManager();


    // Persistence
    void readSettings();
    void writeSettings();

    // String manipulations
    static QString mergeStringList(const QStringList strings);
    static QStringList splitString(const QString split);

    // Rule operations
    static QString ruleKeyWordToString(SubRule sRule);
    static QString ruleSizeLimitsToString(SubRule sRule);
    static QString ruleDateLimitsToString(SubRule sRule);



    // Rule item model
    QList<QTreeWidgetItem*>ruleItems() const;

signals:
    void stateChanged();

private:
    void writeRulesToReg();

    QList<Rule> ruleslist(){return rules;}

    void addRule(const Rule r);
    void addRules(const QList<Rule>r);
    void replaceRule(const Rule r, int index);
    void replaceRule(const Rule r, QString title);
    void swapRule(int i, int j);
    void removeRuleAt(const int i){rules.removeAt(i);}
    void removeRule(const QString title);
    Rule rule(int index) {return rules.value(index);}
    Rule rule(QString title);

    QList<Rule> rules;

    friend class MainApplication;
};

#endif // RULECONTROLLER_H
