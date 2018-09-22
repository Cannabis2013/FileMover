#include "pathline.h"

pathLine::pathLine(QWidget *parent):
    textPathEdit(parent)
{
    connect(line,&QLineEdit::textChanged,this,&pathLine::sendCurrentFilePath);
}

void pathLine::setCurrentFilePath(QString fp)
{
    line->setText(fp);
}

void pathLine::on_toolButton_clicked()
{
    emit showFileExplorer();
}
