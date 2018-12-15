#ifndef ADDRULEDIALOG_H
#define ADDRULEDIALOG_H

#include "abstractRuledialog.h"

class addRuleDialog : public abstractRuleDialog
{
public:
    explicit addRuleDialog(QStringList folderPaths,QWidget *parent = nullptr);

public slots:
signals:

protected slots:
    //void on_treeWidget_doubleClicked(const QModelIndex &index);
private slots:
    void on_addButton_clicked();
    void on_addSubRule_clicked();
    void on_removeSubRule_clicked();
protected:

};

#endif // ADDRULEDIALOG_H
