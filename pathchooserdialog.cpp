#include "pathchooserdialog.h"
#include "ui_pathchooserdialog.h"

pathChooserDialog::pathChooserDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::pathChooserDialog)
{
    ui->setupUi(this);
    fileView = ui->treeView;
    model = new QFileSystemModel(this);
    model->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

    model->setRootPath("c:\\");
    fileView->setModel(model);
    fileView->setColumnWidth(0,128);
    for (int i = 1; i < 4; ++i)
    {
        fileView->setColumnHidden(i,true);
    }
}

pathChooserDialog::~pathChooserDialog()
{
    delete ui;
}

QTreeView *pathChooserDialog::fView()
{
    return fileView;
}

void pathChooserDialog::on_cancelButton_clicked()
{
    close();
}

void pathChooserDialog::on_insertPathButton_clicked()
{
    QModelIndex currentIndex = fileView->currentIndex();
    QString filePath = model->filePath(currentIndex);
    emit chosenPath(filePath);
    this->close();
}
