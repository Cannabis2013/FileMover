#ifndef RULECONTROLLER_H
#define RULECONTROLLER_H

#include <QTreeWidgetItem>
#include "rules.h"

class ruleController
{
public:
    ruleController();

    static QString mergeStringList(const QStringList strings);
    static QStringList splitString(const QString split);

    QList<QTreeWidgetItem*>rulesTreeItems() const;

    void removeRule(const int i){rules.removeAt(i);}

public slots:
    void insertRule(const rule r);
    void insertRules(const QList<rule>r);

private:
    QList<rule> ruleslist(){return rules;}
    QList<rule>*pointerToRules(){return &rules;}

    QList<rule> rules;

    void replaceRule(rule r,int index);

    friend class settingsWindow;
    friend class mainWindow;
};

typedef ruleController rC;

#endif // RULECONTROLLER_H
