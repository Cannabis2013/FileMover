#include "textpathedit.h"
#include "ui_textpathedit.h"

TextPathEdit::TextPathEdit(QWidget *parent, QString txt) :
    MyWidget(parent),
    ui(new Ui::TextPathEdit)
{
    ui->setupUi(this);

    line = ui->pathSelector;
    toolButton = ui->toolButton;

    if(txt != QString())
        line->setText(txt);

    QCompleter *completer = new QCompleter();
    completer->setModel(new QDirModel(completer));

    line->setCompleter(completer);
    installEventFilter(this);
}

TextPathEdit::~TextPathEdit()
{
    delete ui;
}

void TextPathEdit::setLineText(QString txt)
{
    line->setText(txt);
}

QString TextPathEdit::text()
{    
    return line->text();
}

void TextPathEdit::clear()
{
    line->clear();
}

void TextPathEdit::addWidgetToLayout(QWidget *w)
{
    ui->verticalLayout->addWidget(w);
}

QVBoxLayout *TextPathEdit::verticalLayot()
{
    return ui->verticalLayout;
}

void TextPathEdit::insertChosenPath(QString p)
{
    QString currentText = line->text();
    if(currentText.isEmpty())
        currentText = p;
    else
        currentText += ";" + p;

    line->setText(currentText);
}

void TextPathEdit::on_toolButton_clicked()
{
    filepathSelectorDialog *chooseFilePathDialog = new filepathSelectorDialog();
    connect(chooseFilePathDialog,&filepathSelectorDialog::chosenPath,this,&TextPathEdit::insertChosenPath);
    chooseFilePathDialog->show();
}

