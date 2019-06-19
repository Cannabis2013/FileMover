#include "textpathedit.h"
#include "ui_textpathedit.h"

TextPathEdit::TextPathEdit(QWidget *parent, QString txt) :
    MyWidget(parent),
    ui(new Ui::TextPathEdit)
{
    ui->setupUi(this);

    pathSelector = ui->pathSelector;
    toolButton = ui->toolButton;

    if(txt != QString())
        pathSelector->setText(txt);

    QCompleter *completer = new QCompleter();
    completer->setModel(new QDirModel(completer));

    pathSelector->setCompleter(completer);
    installEventFilter(this);
}

TextPathEdit::~TextPathEdit()
{
    delete ui;
}

void TextPathEdit::setLineText(QString txt)
{
    pathSelector->setText(txt);
}

QString TextPathEdit::text()
{    
    return pathSelector->text();
}

void TextPathEdit::clear()
{
    pathSelector->clear();
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
    QString currentText = pathSelector->text();
    if(currentText.isEmpty())
        currentText = p;
    else
        currentText += ";" + p;

    pathSelector->setText(currentText);
}

void TextPathEdit::on_toolButton_clicked()
{
    filepathDialogWidget *chooseFilePathDialog = new filepathDialogWidget();
    connect(chooseFilePathDialog,&filepathDialogWidget::chosenPath,this,&TextPathEdit::insertChosenPath);
    chooseFilePathDialog->show();
}

