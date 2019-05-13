#include "mywidget.h"

MyWidget::MyWidget(QWidget *parent) : QWidget(parent)
{

}

void MyWidget::print(int val, MyWidget::printMode mode)
{
    if(mode == printMode::lineBreak)
        cout << val << endl;
    else
        cout << val;
}

void MyWidget::print(QString text, MyWidget::printMode mode)
{
    if(mode == printMode::lineBreak)
        cout << text.toStdString() << endl;
    else
        cout << text.toStdString();
}

void MyWidget::print(QStringList textList, MyWidget::listMode breakBetweenElemenets, MyWidget::printMode mode)
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
