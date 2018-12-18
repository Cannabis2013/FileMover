#ifndef FILEWORKER_H
#define FILEWORKER_H

#include "fileworkeroperator.h"



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

    void processDirectoryInformation(directoryItem item);
    void processFileInformation(QString path);
    void processFileInformations(QStringList paths);
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
