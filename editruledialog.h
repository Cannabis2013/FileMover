#ifndef EDITRULEDIALOG_H
#define EDITRULEDIALOG_H

#include "addruledialog.h"


class editRuleDialog : public abstractRuleDialog
{
public:
    editRuleDialog(rule editRule, int index, QStringList folderPaths);

signals:


protected slots:
    void on_addButton_clicked();
    void on_addSubRule_clicked();
    void on_removeSubRule_clicked();

private:
    int replaceIndex;

};

#endif // EDITRULEDIALOG_H
