#ifndef FILEWORKER_H
#define FILEWORKER_H

#include "mydatetime.h"
#include "fileinformation.h"
#include "ruledefinitions.h"


extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;

namespace fileOperations {
class processController;
class fileWorker;
}

struct fileObject
{
    long long sz;
    QString path;
};
struct processItems
{
    QFileInfoList list;
    rD::fileActionRule ruleMode = rD::none;
    QStringList destinations;
};

class processController : public QObject
{
    Q_OBJECT
public slots:
    void addToQueue(processItems pI);

signals:
    void wakeUpProcess();
    void processFinished();

protected:
    virtual ~processController();

private:
    void insertItem(processItems pI)
    {
        processQeue.append(pI);
        emit wakeUpProcess();
    }
    processItems takeItem()
    {
        if(processQeue.isEmpty())
            return processItems();
        else
            return processQeue.takeFirst();
    }
    bool queueIsEmpty(){return processQeue.isEmpty();}

    QList<processItems>processQeue;
    friend class fileWorker;
};

class fileWorker : public baseWorker
{
    Q_OBJECT
public:
    explicit fileWorker(processController *pRef = nullptr,QObject *parent = nullptr);
    virtual ~fileWorker();


    static qint64 byteConvert(int unit, QString fromUnit);
    QStringList static createHeader(QFileInfo fi = QFileInfo());

    // Clear related..

    // Count foldersize related..
    long long folderSize(QString pf);

    // Count files/folder related..
    int folderCount(QString p);
    int fileCount(QString p);
    QList<QPair<QString,int> > countFileSuffixes(QString p);


    // String operations..

    static QString mergeStringList(const QStringList strings);
    static QStringList splitString(const QString split);

public slots:

    // Count files & size related..

    void countSize(QStringList l);
    void countFolder(QString path,
                    QDir::Filters f = QDir::NoFilter,
                    QDirIterator::IteratorFlags i = QDirIterator::NoIteratorFlags);
    void countFolders(QStringList Path,
                    QDir::Filters f,
                    QDirIterator::IteratorFlags i);
    // Queue related..

    void handleProcessRequest();

    // Scan directories..
    QTreeWidgetItem *scanDir(QString p);

    // Add directories..
    void processDirectoryInformation(directoryItem item);
    void processFileInformation(QString path);
    void processFileInformations(QStringList paths);

    // Update directories..
signals:
    void itemText(QString iT);
    void antalFiler(long antal);
    void sendSize(QList<fileObject> s);
    void clearFinished(bool a);
    void infoReport(const QString &error);
    void processFinished(directoryItem item);
    void multipleProcessFinished(QList<directoryItem>items);
    void jobDone(bool status);

private slots:

    // Process request..

    // Fileoperation from QFileinfoList..
    bool removeFileItems(const QFileInfoList filePaths);
    bool moveEntities(const QFileInfoList files, const QStringList destinations);
    bool copyEntities(const QFileInfoList files, const QStringList destinations);

protected:



private:

    // Methods..

    // Remove recursively..

    // Process..
    void beginProcess();

    //Member variables..
    bool isBusy;
    QString busyMessage;
    processController *pControllerReference;
};

typedef fileWorker fW;

#endif // FILEWORKER_H
