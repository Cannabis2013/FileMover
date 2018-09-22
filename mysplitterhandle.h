#ifndef MYSPLITTERHANDLE_H
#define MYSPLITTERHANDLE_H

#include <QWidget>
#include <QSplitterHandle>
#include <qpaintengine.h>
#include <QPaintEvent>
#include <iostream>
#include <qlist.h>
#include <qsplitter.h>
#include <myverticalsplitter.h>
#include <qlabel.h>
#include <qpen.h>

using namespace std;

class mySplitterHandle : public QSplitterHandle
{
public:
    mySplitterHandle(Qt::Orientation direction, QSplitter *parent = 0, QString label = QString());

protected:
    void paintEvent(QPaintEvent *event);

private:
    QString handleLabel;
};

#endif // MYSPLITTERHANDLE_H
