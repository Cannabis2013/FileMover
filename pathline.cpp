#include "pathline.h"

PathLine::PathLine(QWidget *parent):
    AbstractTextSelectorDialog(parent)
{

}

void PathLine::on_toolButton_clicked()
{
    emit showFileExplorer();
}
