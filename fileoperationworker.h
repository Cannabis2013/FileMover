#ifndef FILEWORKEROPERATOR_H
#define FILEWORKEROPERATOR_H

#include "rules.h"
#include "entityqueuemanager.h"

#ifdef __MINGW64__
extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;
#endif
class FileOperationWorker : public Worker
{
    Q_OBJECT

public:
    FileOperationWorker();

    QStringList static createHeader(QFileInfo fi = QFileInfo());

    static FileObjectList processFileObjects(FileObjectList fileObjects,SubRule rule);

    static FileObjectList generateFileObjects(const QStringList &paths,
                                              const QString &rPath = QString(),
                                              rD::fileTypeEntity filter = rD::File);

public slots:

    void countNumberOfFolderItems(QString path,
                    QDir::Filters f = QDir::NoFilter,
                    QDirIterator::IteratorFlags i = QDirIterator::NoIteratorFlags);
    void countFolders(QStringList Path);

    void handleProcessRequest();

    // Start/wake-up entity file operations
    void processEntity(EntityModel *entity);

signals:
    void fileCount(long antal);
    void sendFolderSizeEntity(DirectoryCountEntity *fObj);
    void sendFolderSizeEntities(QList<DirectoryCountEntity> s);

    void clearFinished(bool a);

    // Queue related..
    void requestNextEntity();
    void infoReport(const QString &error);
    void processFinished(QList<DirectoryItem>items);
    void jobDone(bool status);


private:

    // File object entity operations

    void processFileActionEntity(EntityModel *entity);
    void processFileInformationEntity(EntityModel *entity);
    void reProcessFileInformationEntity(const QStringList paths);
    void processDirectoryCountEntity(EntityModel *entity);

    // Fileoperation from QFileinfoList..
    bool removeFileItems(const FileObjectList filePaths, QStringList * const err = nullptr);
    bool copyFileItems(const FileObjectList fileObjects, const QStringList destinations, QStringList * const err = nullptr);
    bool moveFileItems(const FileObjectList fileObjects, const QStringList destinations, QStringList * const err = nullptr);

    // Methods to count files and number of contents in folders
    int folderCount(QString p);
    int fileCount(QString p);

    // Methods of calculating size of files and content of folders

    long long folderSize(QString pf);

    // Create a list of suffixes and their occurencies..
    QList<QPair<QString,int> > getListOfSuffixOccuriencies(QString p);

    // Create a list of treeWidgetItem

    QTreeWidgetItem *assembleItemModelsFromPath(QString p);

    QString busyMessage;
    bool isBusy;
};

typedef FileOperationWorker fW;

#endif // FILEWORKEROPERATOR_H
