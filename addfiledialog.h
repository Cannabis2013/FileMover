#ifndef ADDFILEDIALOG_H
#define ADDFILEDIALOG_H

#include "filepathselectordialog.h"
#include <qsizepolicy.h>
#include <QModelIndex>

class addFileDialog : public filepathSelectorDialog
{
    Q_OBJECT
public:
    addFileDialog(QWidget *parent = nullptr);

public slots:
    void hideExplorer();
    void setCurrentIndex(QString txt);

signals:
    void quitSignal();
    void shrinkWindow(QSizePolicy::Policy p,int Height);
    void sendCurrentFilePath(QString fp);
    void addButtonClicked();

protected slots:
    void on_insertPathButton_clicked();
private slots:
    void on_cancelButton_clicked();
    void currentFilePath(QModelIndex fileIndex);

private:
    QFileSystemModel *fileModel;
    QTreeView *view;
};

#endif // ADDFILEDIALOG_H
