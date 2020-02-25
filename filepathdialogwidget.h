#ifndef FILEPATHDIALOGWIDGET_H
#define FILEPATHDIALOGWIDGET_H

#include <QFileSystemModel>
#include <qtreeview.h>
#include <iostream>
#include <QDialog>
#include "abstractframe.h"


namespace Ui {
class filepathSelectorDialog;
}

class filepathDialogWidget : public AbstractFrame
{
    Q_OBJECT

public:
    explicit filepathDialogWidget(Qt::WindowModality modality = Qt::NonModal);

    ~filepathDialogWidget();
    QTreeView *fView();

signals:
    void on_fileView_focusOut();
    void chosenPath(QString p);
    void currentFilePathChanged(QString path);

protected slots:

    virtual void on_cancelButton_clicked();
    virtual void on_insertPathButton_clicked();

protected:
    void resizeNotify(QSize newSize){Q_UNUSED(newSize);}

    QTreeView *fileView;
    QFileSystemModel *model;
    Ui::filepathSelectorDialog *ui;

private slots:
    void handleModelStateChange(const QModelIndex &index);

};
#endif // FILEPATHSELECTORDIALOG_H
