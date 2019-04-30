#ifndef EDITRULEDIALOG_H
#define EDITRULEDIALOG_H

#include "addruledialog.h"


class EditRuleDialog : public AbstractRuleDialog
{
public:
    EditRuleDialog(Rule editRule, QStringList watchFolders);


protected slots:
    void on_addButton_clicked();
    void on_addSubRule_clicked();
    void on_removeSubRule_clicked();

private:
    Rule tempRule;
    QString originalRuleTitle;

};

#endif // EDITRULEDIALOG_H
