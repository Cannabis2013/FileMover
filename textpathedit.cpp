#include "textpathedit.h"
#include "ui_textpathedit.h"

textPathEdit::textPathEdit(QWidget *parent, QString txt) :
    QWidget(parent),
    ui(new Ui::textPathEdit)
{
    ui->setupUi(this);
    line = ui->pathSelector;

    toolButton = ui->toolButton;

    if(txt != QString())
        line->setText(txt);

    QCompleter *completer = new QCompleter();
    completer->setModel(new QDirModel(completer));

    line->setCompleter(completer);
}

textPathEdit::~textPathEdit()
{
    delete ui;
}

void textPathEdit::setLineText(QString txt)
{
    line->setText(txt);
}

QString textPathEdit::text()
{    
    return line->text();
}

void textPathEdit::clear()
{
    line->clear();
}

void textPathEdit::addWidgetToLayout(QWidget *w)
{
    ui->verticalLayout->addWidget(w);
}

QVBoxLayout *textPathEdit::verticalLayot()
{
    return ui->verticalLayout;
}


void textPathEdit::insertChosenPath(QString p)
{
    QString currentText = line->text();
    if(currentText.isEmpty())
        currentText = p;
    else
        currentText += ";" + p;

    line->setText(currentText);
}

void textPathEdit::on_toolButton_clicked()
{
    filepathSelectorDialog *chooseFilePathDialog = new filepathSelectorDialog();
    connect(chooseFilePathDialog,&filepathSelectorDialog::chosenPath,this,&textPathEdit::insertChosenPath);
    chooseFilePathDialog->show();
}

