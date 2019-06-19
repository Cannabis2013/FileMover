#include "addfiledialog.h"

AddFileDialog::AddFileDialog(QWidget *parent):
    filepathDialogWidget()
{
    Q_UNUSED(parent);
    view = fView();
    fileModel = static_cast<QFileSystemModel*>(view->model());


    view->setStyleSheet("QScrollBar::handle"
        "{background-color:rgb(81,81,81);"
        "border:2px solid rgb(81,81,81);"
        "border-radius:5px;}");

    connect(view,&QTreeView::clicked,this,&AddFileDialog::currentFilePath);
}

void AddFileDialog::hideExplorer()
{
    if(!view->isHidden())
    {
        view->hide();
        updateGeometry();
        emit shrinkWindow(QSizePolicy::Fixed,view->height());
    }
    else
    {
        view->show();
        emit shrinkWindow(QSizePolicy::Preferred,128);
    }
}

void AddFileDialog::setCurrentIndex(QString txt)
{
    QModelIndex fileIndex = fileModel->index(txt);
    view->setCurrentIndex(fileIndex);
}

void AddFileDialog::on_insertPathButton_clicked()
{
    emit addButtonClicked();
}

void AddFileDialog::on_cancelButton_clicked()
{
    emit quitSignal();
}

void AddFileDialog::currentFilePath(QModelIndex fileIndex)
{
    QString filePath = fileModel->filePath(fileIndex);
    emit sendCurrentFilePath(filePath);
}
