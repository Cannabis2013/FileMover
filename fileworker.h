#ifndef FILEWORKER_H
#define FILEWORKER_H

#include "processcontroller.h"


extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;

namespace fileOperations {
class processController;
class fileWorker;
}

/*
 * This handles the file operations. It runs in its own thread to ensure multi-tasking capabilities.
*/

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

private:

    // Methods..

    // recursive file operations

    void removeDir(QString &dirName, QStringList &errs);

    // Start pending process in queue..
    void beginProcess();

    //Member variables..
    bool isBusy;
    QString busyMessage;
    processController *pControllerReference;
};

typedef fileWorker fW;

#endif // FILEWORKER_H
