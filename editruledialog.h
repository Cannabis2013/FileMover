#ifndef EDITRULEDIALOG_H
#define EDITRULEDIALOG_H

#include "addruledialog.h"


class EditRuleDialog : public AbstractRuleDialog
{
public:
    EditRuleDialog(const IRule<IDefaultRuleCondition> *editRule, QStringList watchFolders);


protected slots:
    void on_addButton_clicked();
    void on_addSubRule_clicked();
    void on_removeSubRule_clicked();

private:
    void initializeInterface();
    const IRule<IDefaultRuleCondition> *tempRule;
    QString originalRuleTitle;

};

#endif // EDITRULEDIALOG_H
