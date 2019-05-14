#ifndef FILEPATHSELECTORDIALOG_H
#define FILEPATHSELECTORDIALOG_H

#include <QFileSystemModel>
#include <qtreeview.h>
#include <iostream>
#include <QDialog>
#include "frameimplementable.h"


namespace Ui {
class filepathSelectorDialog;
}

class filepathSelectorDialog : public FrameImplementable
{
    Q_OBJECT

public:
    explicit filepathSelectorDialog();

    ~filepathSelectorDialog();
    QTreeView *fView();

signals:
    void on_fileView_focusOut();
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
