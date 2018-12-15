#ifndef FILEWORKER_H
#define FILEWORKER_H

#include "fileworkeroperator.h"

extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;

namespace fileOperations {
class processController;
class fileWorker;
}

/*
 * This handles the file operations. It runs in its own thread to ensure multi-tasking capabilities.
*/

class fileWorker : public fileWorkerOperator
{
    Q_OBJECT
public:
    explicit fileWorker(processController *pRef = nullptr,QObject *parent = nullptr);
    virtual ~fileWorker();

    QStringList static createHeader(QFileInfo fi = QFileInfo());

    // Clear related..

    // Count foldersize related..
    long long folderSize(QString pf);

    // Count files/folder related..
    int folderCount(QString p);
    int fileCount(QString p);
    QList<QPair<QString,int> > countFileSuffixes(QString p);

public slots:

    // Count files & size related..

    void calcSizeOfIndividualFolderItems(QStringList l);
    void countNumberOfFolderItems(QString path,
                    QDir::Filters f = QDir::NoFilter,
                    QDirIterator::IteratorFlags i = QDirIterator::NoIteratorFlags);
    void countFolders(QStringList Path,
                    QDir::Filters f,
                    QDirIterator::IteratorFlags i);
    // Queue related..

    void handleProcessRequest();

    // Scan directories..
    QTreeWidgetItem *scanDir(QString p);

    /*
     * Queues related
     * Add directories
    */

    void processDirectoryInformation(directoryItem item);
    void processFileInformation(QString path);
    void processFileInformations(QStringList paths);

    // Update directories..
signals:
    void clearFinished(bool a);

    // Queue related..
    void infoReport(const QString &error);
    void processFinished(directoryItem item);
    void multipleProcessFinished(QList<directoryItem>items);
    void jobDone(bool status);

private:

    // Methods..
    // Start pending process in queue..
    void beginProcess();

    //Member variables..
    QString busyMessage;
    processController *pControllerReference;
};

typedef fileWorker fW;

#endif // FILEWORKER_H
