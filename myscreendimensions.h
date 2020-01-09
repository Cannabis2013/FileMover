#ifndef MYSCREENDIMENSIONS_H
#define MYSCREENDIMENSIONS_H

#include <qrect.h>

class ScreenDimension: public QRect
{
public:
    ScreenDimension(int left,int tp,int w,int h)
    {
        setLeft(left);
        setTop(tp);
        setWidth(w);
        setHeight(h);
    }
    ScreenDimension(QRect parent = QRect())
    {
        setLeft(parent.left());
        setTop(parent.top());
        setWidth(parent.width());
        setHeight(parent.height());
    }
    bool operator <=(QRect cRect)
    {
        QSize compareDimension(cRect.width(),cRect.height()),
                dimension(this->width(),this->height());

        if(dimension.width() <= compareDimension.width() && dimension.height() <= compareDimension.height())
            return true;
        else
            return false;
    }
};
#endif // MYSCREENDIMENSIONS_H
