#ifndef BASEWORKER_H
#define BASEWORKER_H

#include <QFileInfoList>
#include <QDir>
#include <qdiriterator.h>
#include <QTreeWidgetItem>
#include <qdatetime.h>
#include <qpair.h>

class baseWorker : public QObject
{
public:
    baseWorker(QObject *parent = nullptr);

    virtual ~baseWorker();

    // Rules related..
    enum copyMode{move,copy,noMode};
    enum iteratorMode{filesOnly = 0,folderOnly = 1,allEntries = 2, noTypeSet = 3};

    static double convertSizeToAppropriateUnits(long long numb, QString &denote, int dec = 2);

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
typedef baseWorker bW;

#endif // BASEWORKER_H
