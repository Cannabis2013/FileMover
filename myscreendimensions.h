#ifndef MYSCREENDIMENSIONS_H
#define MYSCREENDIMENSIONS_H

#include <qrect.h>

class myScreenDimension: public QRect
{
public:
    myScreenDimension(int left,int tp,int w,int h)
    {
        setLeft(left);
        setTop(tp);
        setWidth(w);
        setHeight(h);
    }
    myScreenDimension(QRect parent = QRect())
    {
        setLeft(parent.left());
        setTop(parent.top());
        setWidth(parent.width());
        setHeight(parent.height());
    }
    bool operator <=(QRect cRect)
    {
        QSize cSZ(cRect.width(),cRect.height()),
                sZ(this->width(),this->height());

        if(sZ.width() <= cSZ.width() && sZ.height() <= cSZ.height())
            return true;
        else
            return false;
    }
};
#endif // MYSCREENDIMENSIONS_H
