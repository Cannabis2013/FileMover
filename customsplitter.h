#ifndef MYSPLITTER_H
#define MYSPLITTER_H

#include <QWidget>
#include <qsplitter.h>
#include <iostream>
#include "customsplitterhandle.h"
#include <qlayout.h>

using namespace std;

class CustomSplitter : public QSplitter
{
public:
    CustomSplitter(Qt::Orientation direction,QWidget *parent = nullptr);
    CustomSplitter(QWidget *parent = nullptr);
protected:
    QSplitterHandle *createHandle();

private slots:
private:
    int handleCount;
};

#endif // MYSPLITTER_H
