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
    Q_OBJECT
public:
    // Public types
    enum copyMode{move,copy,noMode};
    enum iteratorMode{filesOnly = 0,folderOnly = 1,allEntries = 2, noTypeSet = 3};

    FileWorker();

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
