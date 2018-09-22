#ifndef MYSPLITTER_H
#define MYSPLITTER_H

#include <QWidget>
#include <qsplitter.h>
#include <iostream>
#include "mysplitterhandle.h"
#include <qlayout.h>

using namespace std;

class myVerticalSplitter : public QSplitter
{
public:
    myVerticalSplitter(Qt::Orientation direction,QWidget *parent = 0);
    myVerticalSplitter(QWidget *parent = 0);
protected:
    QSplitterHandle *createHandle();

private slots:
private:
    int handleCount;
};

#endif // MYSPLITTER_H
