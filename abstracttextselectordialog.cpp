#include "abstracttextselectordialog.h"
#include "ui_textpathedit.h"

AbstractTextSelectorDialog::AbstractTextSelectorDialog(QWidget *parent, QString txt) :
    MyWidget(parent),
    ui(new Ui::AbstractTextSelectorDialog)
{
    ui->setupUi(this);

    pathSelector = ui->pathSelector;

    if(txt != QString())
        pathSelector->setText(txt);

    QCompleter *completer = new QCompleter();
    completer->setModel(new QDirModel(completer));

    pathSelector->setCompleter(completer);
    installEventFilter(this);

    connect(pathSelector,&QLineEdit::textChanged,this,&AbstractTextSelectorDialog::sendCurrentFilePath);
}

AbstractTextSelectorDialog::~AbstractTextSelectorDialog()
{
    delete ui;
}

void AbstractTextSelectorDialog::setCurrentFilePath(const QString &fp)
{
    pathSelector->setText(fp);
}

QString AbstractTextSelectorDialog::text()
{    
    return pathSelector->text();
}

void AbstractTextSelectorDialog::clear()
{
    pathSelector->clear();
}

void AbstractTextSelectorDialog::insertChosenPath(QString p)
{
    QString currentText = pathSelector->text();
    if(currentText.isEmpty())
        currentText = p;
    else
        currentText += ";" + p;

    pathSelector->setText(currentText);
}

/*
void TextPathEdit::on_toolButton_clicked()
{
    filepathDialogWidget *chooseFilePathDialog = new filepathDialogWidget();
    connect(chooseFilePathDialog,&filepathDialogWidget::chosenPath,this,&TextPathEdit::insertChosenPath);
    chooseFilePathDialog->show();
}
*/

