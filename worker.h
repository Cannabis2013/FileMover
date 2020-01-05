#ifndef WORKER_H
#define WORKER_H

#include <QFileInfoList>
#include <QDir>
#include <qdiriterator.h>
#include <QTreeWidgetItem>
#include <qdatetime.h>
#include <qpair.h>
#include <math.h>
#include "mutableobject.h"

class Worker
{
public:
    virtual ~Worker();

    // Rules related..
    enum copyMode{move,copy,noMode};
    enum iteratorMode{filesOnly = 0,folderOnly = 1,allEntries = 2, noTypeSet = 3};

    // Convert size units related..
    static double convertSizeToAppropriateUnits(long long numb, QString &denote, int dec = 2);
    static qint64 toBytes(uint unit, QString fromUnit);

    // Basic stringlist operations..
    static QString mergeStringList(const QStringList strings);
    static QStringList splitString(const QString split);

    // Basic string operations..
    QString checkAndCorrectForBackslash(QString path);

    // Filename operations
    static bool containsTrailingBackslash(QString path);
    static QString directoryName(QString path);
    };

#endif // WORKER_H
