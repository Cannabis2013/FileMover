#include "pathline.h"

PathLine::PathLine(QWidget *parent):
    TextPathEdit(parent)
{
    connect(line,&QLineEdit::textChanged,this,&PathLine::sendCurrentFilePath);
}

void PathLine::setCurrentFilePath(QString fp)
{
    line->setText(fp);
}

void PathLine::on_toolButton_clicked()
{
    emit showFileExplorer();
}
