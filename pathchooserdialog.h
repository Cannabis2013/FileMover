#ifndef PATHCHOOSERDIALOG_H
#define PATHCHOOSERDIALOG_H

#include <QWidget>
#include <QFileSystemModel>
#include <qtreeview.h>
#include <iostream>
#include <QTranslator>

namespace Ui {
class pathChooserDialog;
}

class pathChooserDialog : public QWidget
{
    Q_OBJECT

public:
    explicit pathChooserDialog(QWidget *parent = nullptr);

    ~pathChooserDialog();
    QTreeView *fView();



signals:
    void chosenPath(QString p);

protected slots:

    virtual void on_cancelButton_clicked();
    virtual void on_insertPathButton_clicked();

private:

    QTreeView *fileView;
    QFileSystemModel *model;
    Ui::pathChooserDialog *ui;
};
#endif // PATHCHOOSERDIALOG_H
