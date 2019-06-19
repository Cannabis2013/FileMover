#include "pathline.h"

PathLine::PathLine(QWidget *parent):
    TextPathEdit(parent)
{
    connect(pathSelector,&QLineEdit::textChanged,this,&PathLine::sendCurrentFilePath);
}

void PathLine::setCurrentFilePath(QString fp)
{
    pathSelector->setText(fp);
}

void PathLine::on_toolButton_clicked()
{
    emit showFileExplorer();
}
