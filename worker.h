#ifndef WORKER_H
#define WORKER_H

#include <QFileInfoList>
#include <QDir>
#include <qdiriterator.h>
#include <QTreeWidgetItem>
#include <qdatetime.h>
#include <qpair.h>

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

    // Basic stringlist operations..
    static QString mergeStringList(const QStringList strings);
    static QStringList splitString(const QString split);

protected:

    // Move and copy..
    bool moveRecursively(QString path,
                         QString destination);
    bool copyRecursively(QString path,
                         QString destination);

    // Variable manipulation..
    void checkAndCorrectForBackslash(QString *path);
    QString checkAndCorrectForBackslash(QString path);

};
typedef Worker bW;

#endif // WORKER_H
