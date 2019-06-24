#ifndef FILEWORKEROPERATOR_H
#define FILEWORKEROPERATOR_H

#include "rules.h"
#include <QRunnable>

extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;

class FileWorker : public Worker
{
    Q_OBJECT

public:
    FileWorker(EntityQueueManager *pRef);

    QStringList static createHeader(QFileInfo fi = QFileInfo());

    static QFileInfoList processList(QFileInfoList files, SubRule rule);
    static QFileInfoList generateFilesList(QString rPath, QStringList paths,bool recursive = false);

public slots:
    void calcSize(QString path);
    void calcSize(QStringList l);

    void countNumberOfFolderItems(QString path,
                    QDir::Filters f = QDir::NoFilter,
                    QDirIterator::IteratorFlags i = QDirIterator::NoIteratorFlags);
    void countFolders(QStringList Path);

    void handleProcessRequest();

    void processFileInformation(QString path);
    void processFileInformations(QStringList paths);
    void reProcessFileInformation(const QString path);
    void reProcessFileInformations(const QStringList paths);

signals:
    void itemText(QString iT);
    void fileCount(long antal);
    void sendFolderSizeEntity(DirectoryEntity fObj);
    void sendFolderSizeEntities(QList<DirectoryEntity> s);

    void clearFinished(bool a);

    // Queue related..
    void infoReport(const QString &error);
    void processFinished(DirectoryItem item);
    void multipleProcessFinished(QList<DirectoryItem>items);
    void jobDone(bool status);

private:


    // Fileoperation from QFileinfoList..
    bool removeFileItems(const QFileInfoList filePaths);
    bool moveEntities(const QFileInfoList files, const QStringList destinations);
    bool copyEntities(const QFileInfoList files, const QStringList destinations);

    /*
     * Methods related to calc size of folders and the numbers of them
     */

    DirectoryEntity folderContentSize(QString p);
    QList<DirectoryEntity> foldersContentSize(QStringList l);

    // Methods to count files and number of contents in folders
    int folderCount(QString p);
    int fileCount(QString p);

    // Methods of calculating size of files and content of folders

    long long folderSize(QString pf);

    // Create a list of suffixes and their occurencies..
    QList<QPair<QString,int> > getListOfSuffixOccuriencies(QString p);

    // Create a list of treeWidgetItem

    QTreeWidgetItem *scanDir(QString p);

    // Protected member variables;
    bool isBusy;

    void removeDir(QString &dirName, QStringList &errs);
    bool moveRecursively(QString path,
                         QString destination);
    bool copyRecursively(QString path,
                         QString destination);

    void beginProcess();

    QString busyMessage;
    EntityQueueManager *pControllerReference;
};

typedef FileWorker fW;

#endif // FILEWORKEROPERATOR_H
