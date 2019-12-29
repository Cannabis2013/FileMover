#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <qstringlist.h>
#include <iostream>
#include <QTranslator>
#include <QFocusEvent>

using namespace std;


class MutuableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MutuableWidget(QWidget *parent = nullptr);

signals:
    void stateChanged();
};

#endif // MYWIDGET_H
