#ifndef MYHORIZONTALSPLITTER_H
#define MYHORIZONTALSPLITTER_H

#include <QSplitter>

#include "mysplitterhandle.h"

class myHorizontalSplitter : public QSplitter
{
public:
    myHorizontalSplitter(QWidget *parent = nullptr);

protected:
    virtual ~myHorizontalSplitter();
    QSplitterHandle *createHandle();
};

#endif // MYHORIZONTALSPLITTER_H
