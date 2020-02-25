#include "filepathdialogwidget.h"
#include "ui_filepathselectordialog.h"

filepathDialogWidget::filepathDialogWidget(Qt::WindowModality modality) :
    AbstractFrame(),
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

    setWindowModality(modality);

    connect(fileView,&QTreeView::activated,this,&filepathDialogWidget::handleModelStateChange);
}

filepathDialogWidget::~filepathDialogWidget()
{
    delete ui;
}

QTreeView *filepathDialogWidget::fView()
{
    return fileView;
}

void filepathDialogWidget::on_cancelButton_clicked()
{
    close();
}

void filepathDialogWidget::on_insertPathButton_clicked()
{
    QModelIndex currentIndex = fileView->currentIndex();
    QString filePath = model->filePath(currentIndex);
    emit chosenPath(filePath);
    close();
}

void filepathDialogWidget::handleModelStateChange(const QModelIndex &index)
{
    QString currentSelectedFilePath = model->filePath(index);
    emit currentFilePathChanged(currentSelectedFilePath);
}
