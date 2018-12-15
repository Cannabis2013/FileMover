#ifndef FILEPATHSELECTORDIALOG_H
#define FILEPATHSELECTORDIALOG_H

#include <QWidget>
#include <QFileSystemModel>
#include <qtreeview.h>
#include <iostream>
#include <QTranslator>

namespace Ui {
class filepathSelectorDialog;
}

class filepathSelectorDialog : public QWidget
{
    Q_OBJECT

public:
    explicit filepathSelectorDialog(QWidget *parent = nullptr);

    ~filepathSelectorDialog();
    QTreeView *fView();



signals:
    void chosenPath(QString p);

protected slots:

    virtual void on_cancelButton_clicked();
    virtual void on_insertPathButton_clicked();

private:

    QTreeView *fileView;
    QFileSystemModel *model;
    Ui::filepathSelectorDialog *ui;
};
#endif // FILEPATHSELECTORDIALOG_H
