#ifndef WORKER_H
#define WORKER_H

#include <QFileInfoList>
#include <QDir>
#include <qdiriterator.h>
#include <QTreeWidgetItem>
#include <qdatetime.h>
#include <qpair.h>
#include <math.h>

class Worker : public QObject
{
public:
    Worker(QObject *parent = nullptr);

    virtual ~Worker();

    // Rules related..
    enum copyMode{move,copy,noMode};
    enum iteratorMode{filesOnly = 0,folderOnly = 1,allEntries = 2, noTypeSet = 3};

    // Convert size units related..
    static double convertSizeToAppropriateUnits(long long numb, QString &denote, int dec = 2);
    static qint64 byteConvert(int unit, QString fromUnit);


    // Basic stringlist operations..
    static QString mergeStringList(const QStringList strings);
    static QStringList splitString(const QString split);


    // Basic string operations..
    void checkAndCorrectForBackslash(QString *path);
    QString checkAndCorrectForBackslash(QString path);

    // Filename operations

    static bool containsTrailingBackslash(QString path);
    static QString directoryName(QString path);
    };
    typedef Worker wrk;

#endif // WORKER_H
