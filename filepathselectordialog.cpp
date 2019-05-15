#include "filepathselectordialog.h"
#include "ui_filepathselectordialog.h"

filepathSelectorDialog::filepathSelectorDialog() :
    AbstractFrameImplementable(),
    ui(new Ui::filepathSelectorDialog)
{
    ui->setupUi(this);
    fileView = ui->treeView;
    model = new QFileSystemModel(this);
    model->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

    model->setRootPath("c:\\");
    fileView->setModel(model);
    fileView->setColumnWidth(0,128);
    for (int i = 1; i < 4; ++i)
        fileView->setColumnHidden(i,true);
}

filepathSelectorDialog::~filepathSelectorDialog()
{
    delete ui;
}

QTreeView *filepathSelectorDialog::fView()
{
    return fileView;
}

void filepathSelectorDialog::on_cancelButton_clicked()
{
    close();
}

void filepathSelectorDialog::on_insertPathButton_clicked()
{
    QModelIndex currentIndex = fileView->currentIndex();
    QString filePath = model->filePath(currentIndex);
    emit chosenPath(filePath);
    close();
}

void filepathSelectorDialog::resizeEvent(QSize newSize)
{

}
