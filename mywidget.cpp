#include "mywidget.h"

myWidget::myWidget(QWidget *parent) : QWidget(parent)
{

}

void myWidget::print(int val, myWidget::printMode mode)
{
    if(mode == printMode::lineBreak)
        cout << val << endl;
    else
        cout << val;
}

void myWidget::print(QString text, myWidget::printMode mode)
{
    if(mode == printMode::lineBreak)
        cout << text.toStdString() << endl;
    else
        cout << text.toStdString();
}

void myWidget::print(QStringList textList, myWidget::listMode breakBetweenElemenets, myWidget::printMode mode)
{
    for(QString str : textList)
    {
        cout << str.toStdString();
        if(breakBetweenElemenets == listMode::breakLines)
            cout << endl;
    }
    if(mode == printMode::lineBreak)
        cout << endl;
}
