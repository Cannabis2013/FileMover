#ifndef FILEWORKEROPERATOR_H
#define FILEWORKEROPERATOR_H

#include "AbstractFileWorker.h"

#ifdef __MINGW64__
extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;
#endif

namespace FilesContext {
    class FileOperationWorker;
}

class FileWorker : public AbstractFileWorker
{
public:
    // Public types
    enum copyMode{move,copy,noMode};
    enum iteratorMode{filesOnly = 0,folderOnly = 1,allEntries = 2, noTypeSet = 3};
    enum sizeComparemode {LesserThan = 0, LesserOrEqualThan = 1, Equal = 2, greaterOrEqualThan = 3, greaterThan = 4};
    enum dateCompareMode {YoungerThan = 0, YoungerOrExactThan = 1, Exact = 2, OlderOrExtactThan = 3, OlderThan = 4};
    enum fileDateMode {DateCreated = 0, DateEdited = 1};
    FileWorker();


    // Generate stringlists

    QStringList filterFilesThatMatch(const QStringList &filePaths,QString &str, bool dontMatch = false, bool suffix = false);
    QStringList filterFilesThatContain(const QStringList &filePaths,QString &str, bool dontContain = false, bool suffix = false);
    QStringList filterFilesAccordingToSize(const QStringList &filePaths, int &size,int mode = 0);
    QStringList filterFilesAccordingToDate(const QStringList &filePaths, int day, int month, int year, const int &mode = 0, const int &dateMode = 0);

    FileObjectList processFileObjects(const FileObjectList &fileObjects,const SubRule &rule) override;

    FileObjectList generateFileObjects(const QStringList &paths,
                                              const QString &rPath = QString(),
                                              const RRT::FileTypeEntity &filter = RRT::File)  override;

    void countFolderItems(const QString &path,
                    const QDir::Filters &filters = QDir::NoFilter,
                    const QDirIterator::IteratorFlags &flags = QDirIterator::NoIteratorFlags) override;
    void countFolders(const QStringList &Path) override;

    // wake-up/Start entity file operations
    void handleProcessRequest() override;
    void processEntity(IModelDelegate<EntityModel, EntityType> *delegate) override;

private:
    // File object entity operations

    void processFileEntity(const IModelDelegate<FileRuleEntity, EntityType> *delegate);
    void processFileInformationEntity(const IModelDelegate<FileInformationEntity, EntityType> *delegate);
    void processDirectoryCountEntity(const IModelDelegate<DirectoryEntity,EntityType> *delegate);
    void reProcessFileInformationEntity(const QStringList &paths);

    // Fileoperation from QFileinfoList..
    bool removeFileItems(const FileObjectList& filePaths, QStringList * const err = nullptr);
    bool copyFileItems(const FileObjectList fileObjects, const QStringList destinations, QStringList * const err = nullptr);
    bool moveFileItems(const FileObjectList fileObjects, const QStringList destinations, QStringList * const err = nullptr);

    /* Methods to count files and number of contents in folders
     *  - Please note they all relies on QObject functionalities
     */

    int folderCount(QString p);
    int fileCountFromPath(QString p);

    // Methods of calculating size of files and content of folders

    qint64 folderSize(const QString &path);

    QString busyMessage;
    bool isBusy;
};

typedef FileWorker fW;

#endif // FILEWORKEROPERATOR_H
