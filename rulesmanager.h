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


    // State manipulation
    void swapRule(int i, int j);
    void addRule(const Rule r);
    void addRules(const QList<Rule>r);
    void replaceRule(const Rule r, int index);
    void replaceRule(const Rule r, QString title);
    void removeRuleAt(const int i){rules.removeAt(i);}
    void removeRule(const QString title);


    // Get
    const QList<Rule> ruleslist() {return rules;}
    const Rule rule(int index) {return rules.value(index);}
    const Rule rule(QString title);
    int ruleCount() {return rules.count();}

    // Get generic models
    QList<QTreeWidgetItem*>ruleItems() const;

signals:
    void stateChanged();

private:

    QList<Rule> rules;

};

#endif // RULECONTROLLER_H
