#ifndef EDITRULEDIALOG_H
#define EDITRULEDIALOG_H

#include "addruledialog.h"


class EditRuleDialog : public AbstractRuleDialog
{
public:
    EditRuleDialog(const DefaultRuleInterface *editRule, QStringList watchFolders);


protected slots:
    void on_addButton_clicked();
    void on_addSubRule_clicked();
    void on_removeSubRule_clicked();

private:
    void initializeInterface();
    const DefaultRuleInterface *tempRule;
    QString originalRuleTitle;

};

#endif // EDITRULEDIALOG_H
