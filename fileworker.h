#ifndef FILEWORKER_H
#define FILEWORKER_H

#include "rules.h"

namespace fileOperations {
class processController;
class fileWorker;
}

/*
 * This handles the file operations. It runs in its own thread to ensure multi-tasking capabilities.
*/

class FileWorker : public fileWorkerOperator
{
    Q_OBJECT
public:
    explicit FileWorker(ProcessManager *pRef = nullptr,QObject *parent = nullptr);
    virtual ~FileWorker();

    QFileInfoList processList(QFileInfoList files, SubRule rule);

public slots:
    // Count files & size related..

    void calcSizeOfIndividualFolderItems(QStringList l);
    void countNumberOfFolderItems(QString path,
                    QDir::Filters f = QDir::NoFilter,
                    QDirIterator::IteratorFlags i = QDirIterator::NoIteratorFlags);
    void countFolders(QStringList Path);
    // Queue related..

    void handleProcessRequest();

    /* 
     * Add directories
    */

    void processFileInformation(QString path);
    void processFileInformations(QStringList paths);
signals:
    void clearFinished(bool a);

    // Queue related..
    void infoReport(const QString &error);
    void processFinished(DirectoryItem item);
    void multipleProcessFinished(QList<DirectoryItem>items);
    void jobDone(bool status);

private:

    // Methods..
    // Start pending process in queue..
    void beginProcess();

    //Member variables..
    QString busyMessage;
    ProcessManager *pControllerReference;
};


typedef FileWorker fW;
#endif // FILEWORKER_H

