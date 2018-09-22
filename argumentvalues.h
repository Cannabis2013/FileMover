#ifndef ARGUMENTVALUES_H
#define ARGUMENTVALUES_H

#include <qobject.h>

struct altArgsContainer
{
    bool developerMode = false;
    QString alternateIconPath = QString();
    QStringList additionalFilePaths = QStringList();
};

#endif // ARGUMENTVALUES_H
