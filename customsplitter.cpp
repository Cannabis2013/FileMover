#include "customsplitter.h"

CustomSplitter::CustomSplitter(Qt::Orientation direction, QWidget *parent):
    QSplitter(direction,parent)
{
    handleCount = 0;
    setContentsMargins(0,0,0,0);
    setStyleSheet("margin-left:0px;");
}

CustomSplitter::CustomSplitter(QWidget *parent):
    QSplitter(parent)
{
    setOrientation(Qt::Vertical);
    handleCount = 0;
    setContentsMargins(0,0,0,0);
    setStyleSheet("margin-left:0px;");
}

QSplitterHandle *CustomSplitter::createHandle()
{
    QStringList labels {"Detailedview","Extention list"};
    QString txt;
    if(handleCount == 1)
        txt = labels.at(0);
    else if(handleCount == 2)
        txt = labels.at(1);
    handleCount++;
    return new CustomSplitterHandle(orientation(),this,txt);
}
