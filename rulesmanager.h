#ifndef RULECONTROLLER_H
#define RULECONTROLLER_H

#include <QListWidgetItem>

#include <qapplication.h>

#include "abstractpersistence.h"
#include "rules.h"

class rulesManager : public QObject,
        public AbstractPersistence
{

    Q_OBJECT
public:
    rulesManager(QString appName, QString orgName);
    ~rulesManager();

    void readSettings();
    void writeSettings();

    static QString mergeStringList(const QStringList strings);
    static QStringList splitString(const QString split);

    static QString ruleKeyWordToString(SubRule sRule);
    static QString ruleSizeLimitsToString(SubRule sRule);
    static QString ruleDateLimitsToString(SubRule sRule);

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
