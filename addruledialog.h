#ifndef ADDRULEDIALOG_H
#define ADDRULEDIALOG_H

#include "abstractRuleDialog.h"

class addRuleDialog : public abstractRuleDialog
{
public:
    explicit addRuleDialog(QStringList folderPaths,QWidget *customFrame = nullptr);
public slots:
signals:

protected slots:
    void on_treeWidget_doubleClicked(const QModelIndex &index);
private slots:
    void on_addButton_clicked();
    void on_addSubRule_clicked();
    void on_removeSubRule_clicked();
protected:

private:


};

#endif // ADDRULEDIALOG_H
