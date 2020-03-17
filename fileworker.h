#ifndef FILEWORKEROPERATOR_H
#define FILEWORKEROPERATOR_H

#include "AbstractFileWorker.h"

#ifdef __MINGW64__
extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;
#endif

namespace FilesContext
{
    class FileWorker;
}

class FileWorker :
        public AbstractFileWorker
{
public:
    // Public types
    FileWorker();

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
    bool removeFileItems(const FileModelList& filePaths, QStringList * const err = nullptr);
    bool copyFileItems(const FileModelList fileObjects, const QStringList destinations, QStringList * const err = nullptr);
    bool moveFileItems(const FileModelList fileObjects, const QStringList destinations, QStringList * const err = nullptr);

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
