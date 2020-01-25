#ifndef WORKER_H
#define WORKER_H

#include <QFileInfoList>
#include <QDir>
#include <qdiriterator.h>
#include <QTreeWidgetItem>
#include <qdatetime.h>
#include <qpair.h>
#include <math.h>
#include "imutableobject.h"

class Worker
{
public:
    virtual ~Worker();

    // Rules related..
    enum copyMode{move,copy,noMode};
    enum iteratorMode{filesOnly = 0,folderOnly = 1,allEntries = 2, noTypeSet = 3};

    // Convert size units related..
    static double convertFromBytes(long long numb, QString &denote, int dec = 2);
    static qint64 convertToBytes(uint unit, QString fromUnit);

    // Basic stringlist operations..
    static QString mergeStringList(const QStringList strings);
    static QStringList splitString(const QString split);

    // Basic string operations..
    QString checkAndCorrectForBackslash(QString path);

    // Filename operations
    static bool hasTrailingBackslash(QString path);
    static QString directoryName(QString path);
    };

#endif // WORKER_H
