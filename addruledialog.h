#ifndef ADDRULEDIALOG_H
#define ADDRULEDIALOG_H

#include "abstractruledialog.h"
#include "messageBox.h"
#include <QTranslator>

class
        AddRuleDialog : public AbstractRuleDialog
{
public:
    explicit AddRuleDialog(QStringList watchFolders, IRuleDefinitions<RRT::RuleType, RRT::RuleAction, RRT::RuleCriteria, RRT::RuleCompareCriteria, RRT::FileTypeEntity> *service);
public slots:
signals:

protected slots:
    void on_treeWidget_doubleClicked(const QModelIndex &index);
private slots:
    void on_addButton_clicked();
    void on_addSubRule_clicked();
    void on_removeSubRule_clicked();

private:
};

#endif // ADDRULEDIALOG_H
