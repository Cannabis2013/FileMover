#include "addfiledialog.h"

addFileDialog::addFileDialog(QWidget *parent):
    pathChooserDialog(parent)
{
    view = fView();
    fileModel = (QFileSystemModel*) view->model();


    view->setStyleSheet("QScrollBar::handle"
        "{background-color:rgb(81,81,81);"
        "border:2px solid rgb(81,81,81);"
        "border-radius:5px;}");

    connect(view,&QTreeView::clicked,this,&addFileDialog::currentFilePath);
}

void addFileDialog::hideExplorer()
{
    if(!view->isHidden())
    {
        view->hide();
        emit shrinkWindow(QSizePolicy::Fixed,72);
    }
    else
    {
        view->show();
        emit shrinkWindow(QSizePolicy::Preferred,128);
    }
}

void addFileDialog::setCurrentIndex(QString txt)
{
    QModelIndex fileIndex = fileModel->index(txt);
    view->setCurrentIndex(fileIndex);
}

void addFileDialog::on_insertPathButton_clicked()
{
    emit addButtonClicked();
}

void addFileDialog::on_cancelButton_clicked()
{
    emit quitSignal();
}

void addFileDialog::currentFilePath(QModelIndex fileIndex)
{
    QString filePath = fileModel->filePath(fileIndex);
    emit sendCurrentFilePath(filePath);
}
