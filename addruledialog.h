#ifndef ADDRULEDIALOG_H
#define ADDRULEDIALOG_H

#include "AbstractRuleDialog.h"
#include "messageBox.h"
#include <QTranslator>

class AddRuleDialog : public AbstractRuleDialog
{
public:
    explicit AddRuleDialog(QStringList watchFolders);
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
