#ifndef ABSTRACTWINDOWMANAGER_H
#define ABSTRACTWINDOWMANAGER_H

#include <QString>
#include <qdiriterator.h>
#include <qfileinfo.h>

#include "MyIcon.h"

class IconScanner
{
public:
    IconScanner();
    ~IconScanner();

protected:
    QList<MyIcon> iconsFromPath(QString path);
};

#endif // ABSTRACTWINDOWMANAGER_H
