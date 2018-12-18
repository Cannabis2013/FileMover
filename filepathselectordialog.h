#ifndef FILEPATHSELECTORDIALOG_H
#define FILEPATHSELECTORDIALOG_H

#include <QFileSystemModel>
#include <qtreeview.h>
#include <iostream>
#include <QDialog>

#include "mywidget.h"


namespace Ui {
class filepathSelectorDialog;
}

class filepathSelectorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit filepathSelectorDialog();

    ~filepathSelectorDialog();
    QTreeView *fView();



signals:
    void chosenPath(QString p);

protected slots:

    virtual void on_cancelButton_clicked();
    virtual void on_insertPathButton_clicked();

protected:

    QTreeView *fileView;
    QFileSystemModel *model;
    Ui::filepathSelectorDialog *ui;
};
#endif // FILEPATHSELECTORDIALOG_H
