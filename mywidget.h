#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <qstringlist.h>
#include <iostream>
#include <QTranslator>
#include <QFocusEvent>

using namespace std;


class MyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyWidget(QWidget *parent = nullptr);
    enum printMode{lineBreak,noLineBreak};
    enum listMode{noBreak,breakLines};
    void print(int val, printMode mode = printMode::lineBreak);
    void print(QString text, printMode mode = printMode::lineBreak);
    void print(QStringList textList, listMode breakBetweenElemenets = listMode::noBreak ,printMode mode = printMode::lineBreak);

signals:

public slots:

protected:

};

#endif // MYWIDGET_H
