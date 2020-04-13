#ifndef FILEWORKEROPERATOR_H
#define FILEWORKEROPERATOR_H

#include "AbstractFileWorker.h"
#include "imodelbuilder.h"

#include "filescontext.h"

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
    void processEntity(DefaultModelInterface *model) override;

private:
    // File object entity operations

    void processFileEntity(const DefaultModelInterface *model);
    void processFileInformationEntity(const DefaultModelInterface *model);
    void processDirectoryCountEntity(const DefaultModelInterface *model);
    void reProcessFileInformationEntity(const QStringList &paths);

    // Fileoperation from QFileinfoList..
    bool removeFileItems(const DefaultFileModelList& filePaths, QStringList * const err = nullptr);
    bool copyFileItems(const DefaultFileModelList fileObjects, const QStringList destinations, QStringList * const err = nullptr);
    bool moveFileItems(const DefaultFileModelList fileObjects, const QStringList destinations, QStringList * const err = nullptr);

    /* Methods to count files and number of contents in folders
     *  - Please note they all relies on QObject functionalities
     */

    int folderCount(QString p);
    int fileCountFromPath(QString p);

    // Methods of calculating size of files and content of folders

    qint64 folderSize(const QString &path);

    QString busyMessage;
    bool isBusy;
    IModelBuilder<IFileModel<QFileInfo>,QString>* fileModelBuilderService;

};

typedef FileWorker fW;

#endif // FILEWORKEROPERATOR_H
