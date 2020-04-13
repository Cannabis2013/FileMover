#include "fileworker.h"

FileWorker::FileWorker()
{
    busyMessage = "Luke Fileworker is busy. Patience my young padawan.";
    isBusy = false;
    #ifdef __WIN32__
        qt_ntfs_permission_lookup++;
    #elif __WIN64__
        qt_ntfs_permission_lookup++;
#endif
}

bool FileWorker::removeFileItems(const DefaultFileModelList& filePaths, QStringList *const err)
{
    if(filePaths.isEmpty())
        return true;

    for(auto model: filePaths)
    {
        auto fileInfo = model->fileInterface();
        QString absoluteFilePath = fileInfo.absoluteFilePath();
        if(model->fileInterface().isFile())
        {
            QFile fileItem(fileInfo.absoluteFilePath());
            if(!fileItem.remove())
            {
                *err << "Operation on: " + fileInfo.fileName() + " in: " +
                    fileInfo.absolutePath() + " returned: " + fileItem.errorString();
            }
        }
        else if(fileInfo.isDir())
        {
            removeFileItems(model->children(),err);
            QDir dir(absoluteFilePath);
            dir.rmdir(absoluteFilePath);
        }
    }

    return true;
}

bool FileWorker::moveFileItems(const DefaultFileModelList fileObjects, const QStringList destinations, QStringList * const err)
{

    // TODO: Implement some error handling when something goes wrong
    bool result = true;
    for(QString destPath : destinations)
    {
        QDir dir(destPath);
        if(!dir.exists())
            dir.mkdir(destPath);

        for(auto model : fileObjects)
        {
            bool noErrors = true;
            auto fileInfo = model->fileInterface();
            QString AbsoluteFilePath = FCU::checkAndCorrectForBackslash(destPath) + fileInfo.fileName();
            if(fileInfo.isDir())
            {
                noErrors = moveFileItems(model->children(),QStringList() << AbsoluteFilePath,err);
                result = noErrors ? result : false;
            }
            else if(fileInfo.isFile())
            {
                QFile file(fileInfo.absoluteFilePath());
                QString errString;
                noErrors = file.copy(AbsoluteFilePath);
                result = noErrors ? result : false;
                if(!noErrors)
                {
                    *err << QString("Failed to delete file '%1'. Error provided: %2")
                            .arg(fileInfo.fileName()).arg(file.errorString());
                }
            }
            else if(!fileInfo.exists())
                continue;
            if(noErrors)
                removeFileItems(DefaultFileModelList() << fileModelBuilderService->buildModel(new QString(model->filepath())) ,err);
        }
    }
    return result;
}

bool FileWorker::copyFileItems(const DefaultFileModelList fileObjects, const QStringList destinations, QStringList * const err)
{
    // TODO: Implement some error handling when something goes wrong
    Q_UNUSED(err);
    bool result = true;
    for(QString destPath : destinations)
    {
        QDir dir(destPath);
        if(!dir.exists())
            dir.mkdir(destPath);

        for(auto model : fileObjects)
        {
            bool noErrors = true;
            auto fileInfo = model->fileInterface();
            QString AbsoluteFilePath = FCU::checkAndCorrectForBackslash(destPath) + fileInfo.fileName();
            if(fileInfo.isDir())
            {
                noErrors = copyFileItems(model->children(),QStringList() << AbsoluteFilePath);
                result = noErrors ? result : false;
            }
            else if(fileInfo.isFile())
            {
                noErrors = QFile::copy(fileInfo.absoluteFilePath(),AbsoluteFilePath);
                result = noErrors ? result : false;
            }
            else if(!fileInfo.exists())
                continue;
        }
    }
    return result;
}

void FileWorker::processDirectoryCountEntity(const IModelDelegate<DirectoryEntityModel, DefaultEntityType> *delegate)
{
    auto entity = new DirectoryEntityModel(*delegate->model());

    QFileInfo fInfo = entity->directoryPath;

    entity->directorySize = folderSize(fInfo.absoluteFilePath());

    emit sendFolderSizeEntity(entity);
}

void FileWorker::processEntity(DefaultModelInterface *model)
{
    isBusy = true;
    if(model->type() == EntityModelContext::NullEntity)
    {
        emit jobDone(true);
        isBusy = false;
        return;
    }

    if(model->type() == EntityModelContext::FileRuleEntity)
    {
        try {
            processFileEntity(model);
        } catch (InheritExceptionDelegate<EntityModel,FileRuleEntityModel<>> *e) {
            throw e->what();
        }
    }
    else if(model->type() == EntityModelContext::FileInformationEntity)
    {
        try {
            processFileInformationEntity(model);
        } catch (const InheritExceptionDelegate<EntityModel,FileInformationEntityModel> *e) {
            throw e->what();
        }
    }
    else if(delegate->type() == EntityModel::directoryCountEntity)
    {
        try {
            processDirectoryCountEntity(DelegateBuilder::buildDelegate<DirectoryEntityModel>(delegate->model()));
        } catch (const InheritExceptionDelegate<EntityModel,DirectoryEntityModel> *e) {
            throw e->what();
        }
    }

    delegate = nullptr;
    isBusy = false;

    emit requestNextEntity();
}

int FileWorker::folderCount(QString p)
{
    QFileInfo info = p;
    QDirIterator ite(info.absoluteFilePath(),
                     QDir::NoDotAndDotDot |
                     QDir::NoDot | QDir::AllEntries,
                     QDirIterator::Subdirectories);
    int taeller = 0;
    while(ite.hasNext())
    {
        if(QFileInfo(ite.fileInfo()).isDir())
            taeller++;
        emit sendStatusLineMessage(QString("%1 folders").arg(QString::number(taeller)));
        ite.next();
    }
    return taeller;
}

int FileWorker::fileCountFromPath(QString p)
{
    QDirIterator ite(QFileInfo(p).absoluteFilePath(),
                     QDir::NoDotAndDotDot |
                     QDir::NoDot | QDir::Files,
                     QDirIterator::Subdirectories);
    int taeller = 0;
    while(ite.hasNext())
    {
        if(QFileInfo(ite.next()).isFile())
            taeller++;
        emit sendStatusLineMessage(QString("%1 files").arg(QString::number(taeller)));
    }
    return taeller;
}

qint64 FileWorker::folderSize(const QString &path)
{
    long long sZ = 0;

    QDirIterator iT(QFileInfo(path).absoluteFilePath(),
                    QDir::NoDotAndDotDot |
                    QDir::Files | QDir::System |
                    QDir::Hidden,
                    QDirIterator::Subdirectories);
    while(iT.hasNext())
     {
         QString fP = iT.next();

         QFile f(fP);
         if(f.open(QIODevice::ReadOnly))
         {
            sZ += f.size();
            emit sendStatusLineMessage( "Calculating folder size: " + f.fileName());
         }
         f.close();
     }
    return sZ;
}

void FileWorker::handleProcessRequest()
{
    if(isBusy)
        return;
    else
        emit requestNextEntity();

}

void FileWorker::processFileInformationEntity(const DefaultModelInterface *model)
{
    auto entity = dynamic_cast<const FileInformationEntityModel*>(model);

    QList<DirectoryItem> directories;
    for (auto path : entity->filePaths())
    {
        DirectoryItem item;

        QString denotation;
        item.path = path;
        double directorySize = FCU::convertFromBytes(folderSize(path),denotation);
        item.dirSize = QString::number(directorySize) + " " + denotation;
        item.numberOfDirectories = folderCount(path);
        item.numberOfFiles = fileCountFromPath(path);
        item.directoryContent = QDir(path).entryInfoList(QDir::AllEntries |
                                                         QDir::System |
                                                         QDir::Hidden);
        item.directoryItemModels = ByteCollection::assembleItemModelsFromPath(path);
        item.sufList = ByteCollection::getListOfSuffixOccuriencies(path);
        directories << item;
    }

    emit processFinished(directories);
}

void FileWorker::reProcessFileInformationEntity(const QStringList &paths)
{
    QList<DirectoryItem>directories;
    for(const QString &p : paths)
    {
        QString denotation;
        DirectoryItem item;
        item.path = p;
        double directorySize = FCU::convertFromBytes(folderSize(p),denotation);
        item.dirSize = QString::number(directorySize) + " " + denotation;
        item.numberOfDirectories = folderCount(p);
        item.numberOfFiles = fileCountFromPath(p);
        item.directoryItemModels = ByteCollection::assembleItemModelsFromPath(p);
        item.sufList = ByteCollection::getListOfSuffixOccuriencies(p);
        directories << item;
    }
    emit processFinished(directories);
}

void FileWorker::processFileEntity(const DefaultModelInterface *model)
{
    auto entity = dynamic_cast<const FileRuleEntityModel<>*>(model);
    if(entity->ruleAction() == FilesContext::Delete || entity->ruleAction() == FilesContext::None)
    {
        // TODO: You have to pass an error related stringlist in order to be able to display errors
        removeFileItems(entity->files());

        processFileInformationEntity(DelegateBuilder::buildDelegate<FileInformationEntityModel>(entity));
    }
    else if(model->fileActionRule == FilesContext::Move)
    {
        moveFileItems(model->allFiles,model->fileDestinations);
        auto entity = DelegateBuilder::buildFileInformationEntity
                <FileInformationEntityModel>(model->directoryPaths)->model();

        processFileInformationEntity(DelegateBuilder::buildDelegate<FileInformationEntityModel>(entity));

    }
    else if(model->fileActionRule == FilesContext::Copy)
        copyFileItems(model->allFiles,model->fileDestinations);

    model = nullptr;
}
