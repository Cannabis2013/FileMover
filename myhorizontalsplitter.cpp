#include "myhorizontalsplitter.h"

myHorizontalSplitter::myHorizontalSplitter(QWidget *parent)
{
    setParent(parent);
    setOrientation(Qt::Horizontal);
    setContentsMargins(0,0,0,0);
}

myHorizontalSplitter::~myHorizontalSplitter()
{
    delete this;
}

QSplitterHandle *myHorizontalSplitter::createHandle()
{
    return new mySplitterHandle(orientation(),this);
}
