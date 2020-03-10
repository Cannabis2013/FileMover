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

bool FileWorker::removeFileItems(const FileObjectList& filePaths, QStringList *const err)
{
    if(filePaths.isEmpty())
        return true;

    for(ITreeModelDelegate<FileModel,DefaultModelType>* modelDelegate: filePaths)
    {
        const FileModel *model = static_cast<const FileModel*>(modelDelegate->model());
        QString absoluteFilePath = model->absoluteFilePath();
        if(model->isFile())
        {
            QFile fileItem(model->absoluteFilePath());
            if(!fileItem.remove())
            {
                *err << "Operation on: " + model->fileName() + " in: " +
                    model->absolutePath() + " returned: " + fileItem.errorString();
            }
        }
        else if(model->isDir())
        {
            removeFileItems(modelDelegate->children(),err);
            QDir dir(absoluteFilePath);
            dir.rmdir(absoluteFilePath);
        }
    }

    return true;
}

bool FileWorker::moveFileItems(const FileObjectList fileObjects, const QStringList destinations, QStringList * const err)
{

    // TODO: Implement some error handling when something goes wrong
    bool result = true;
    for(QString destPath : destinations)
    {
        QDir dir(destPath);
        if(!dir.exists())
            dir.mkdir(destPath);

        for(ITreeModelDelegate<FileModel,DefaultModelType>* modelDelegate : fileObjects)
        {
            bool noErrors = true;
            auto model = static_cast<const FileModel*>(modelDelegate->model());
            QString AbsoluteFilePath = StaticStringCollections::checkAndCorrectForBackslash(destPath) + model->fileName();
            if(model->isDir())
            {
                noErrors = moveFileItems(modelDelegate->children(),QStringList() << AbsoluteFilePath,err);
                result = noErrors ? result : false;
            }
            else if(model->isFile())
            {
                QFile file(model->absoluteFilePath());
                QString errString;
                noErrors = file.copy(AbsoluteFilePath);
                result = noErrors ? result : false;
                if(!noErrors)
                {
                    *err << QString("Failed to delete file '%1'. Error provided: %2")
                            .arg(model->fileName()).arg(file.errorString());
                }
            }
            else if(!model->exists())
                continue;
            if(noErrors)
                removeFileItems(FileObjectList() << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath()),err);
        }
    }
    return result;
}

bool FileWorker::copyFileItems(const FileObjectList fileObjects, const QStringList destinations, QStringList * const err)
{
    // TODO: Implement some error handling when something goes wrong
    Q_UNUSED(err);
    bool result = true;
    for(QString destPath : destinations)
    {
        QDir dir(destPath);
        if(!dir.exists())
            dir.mkdir(destPath);

        for(const ITreeModelDelegate<FileModel,DefaultModelType>* modelDelegate : fileObjects)
        {
            bool noErrors = true;
            const FileModel* model = static_cast<const FileModel*>(modelDelegate->model());
            QString AbsoluteFilePath = StaticStringCollections::checkAndCorrectForBackslash(destPath) + model->fileName();
            if(model->isDir())
            {
                noErrors = copyFileItems(modelDelegate->children(),QStringList() << AbsoluteFilePath);
                result = noErrors ? result : false;
            }
            else if(model->isFile())
            {
                noErrors = QFile::copy(model->absoluteFilePath(),AbsoluteFilePath);
                result = noErrors ? result : false;
            }
            else if(!model->exists())
                continue;
        }
    }
    return result;
}

void FileWorker::processDirectoryCountEntity(const IModelDelegate<DirectoryEntity, EntityType> *delegate)
{
    auto entity = new DirectoryEntity(*delegate->model());

    QFileInfo fInfo = entity->directoryPath;

    entity->directorySize = folderSize(fInfo.absoluteFilePath());

    emit sendFolderSizeEntity(entity);
}

void FileWorker::processEntity(IModelDelegate<EntityModel,EntityType> *delegate)
{
    isBusy = true;
    if(delegate->type() == EntityModel::nullEntity)
    {
        emit jobDone(true);
        isBusy = false;
        return;
    }

    if(delegate->type() == EntityModel::fileActionEntity)
    {
        try {
            processFileEntity(DelegateBuilder::buildDelegate<FileRuleEntity>(delegate->model()));
        } catch (InheritExceptionDelegate<EntityModel,FileRuleEntity> *e) {
            throw e->what();
        }
    }
    else if(delegate->type() == EntityModel::fileInformationEntity)
    {
        try {
            processFileInformationEntity(DelegateBuilder::buildDelegate<FileInformationEntity>(delegate->model()));
        } catch (const InheritExceptionDelegate<EntityModel,FileInformationEntity> *e) {
            throw e->what();
        }
    }
    else if(delegate->type() == EntityModel::directoryCountEntity)
    {
        try {
            processDirectoryCountEntity(DelegateBuilder::buildDelegate<DirectoryEntity>(delegate->model()));
        } catch (const InheritExceptionDelegate<EntityModel,DirectoryEntity> *e) {
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

FileObjectList FileWorker::processFileObjects(const FileObjectList &fileObjects, const SubRule &rule)
{
    FileObjectList filesToProcess;
    for(ITreeModelDelegate<FileModel,DefaultModelType>* modelDelegate : fileObjects)
    {
        auto model = modelDelegate->model();
        bool condition = false;
        // Evaluating filename patterns
        if(rule.criteria() == RRT::fileBaseMode)
        {
            if(rule.compareCriteria() == RRT::contains)
            {
                for(QString kWord : rule.keyWords())
                {
                    if(model->baseName().contains(kWord))
                    {
                        filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
                        break;
                    }
                }
            }
            else if(rule.compareCriteria() == RRT::dontContain)
            {
                for(QString kWord : rule.keyWords())
                {
                    if(model->baseName().contains(kWord))
                    {
                        filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
                        break;
                    }
                }
            }
            else if(rule.compareCriteria() == RRT::match)
            {
                for(QString kWord : rule.keyWords())
                {
                    if(model->baseName() == kWord)
                    {
                        filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
                        break;
                    }
                }
            }
            else if(rule.compareCriteria() == RRT::dontMatch)
            {
                for(QString kWord : rule.keyWords())
                    if(model->baseName() == kWord)
                        condition = true;
                if(!condition)
                    filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
            }
        }
        else if(rule.criteria() == RRT::filepathMode)
        {
            if(rule.compareCriteria() == RRT::contains)
            {
                for(QString kWord : rule.keyWords())
                {
                    if(model->fileName().contains(kWord))
                    {
                        filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
                        break;
                    }
                }
            }
            else if(rule.compareCriteria() == RRT::dontContain)
            {
                for(QString kWord : rule.keyWords())
                {
                    if(model->fileName().contains(kWord))
                        condition = true;
                }
                if(!condition)
                    filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
            }
            else if(rule.compareCriteria() == RRT::match)
            {
                for(QString kWord : rule.keyWords())
                    if(model->fileName() == kWord)
                    {
                        filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
                        break;
                    }
            }
            else if(rule.compareCriteria() == RRT::dontMatch)
            {
                for(QString kWord : rule.keyWords())
                    if(model->fileName() == kWord)
                        condition = true;
                if(!condition)
                    filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
            }
        }

        // Evaluating file extension related patterns
        else if(rule.criteria() == RRT::fileExtensionMode && model->isFile())
        {
            if(rule.compareCriteria() == RRT::contains)
            {
                for(QString kWord : rule.keyWords())
                    if(model->suffix().contains(kWord))
                    {
                        filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
                        break;
                    }
            }
            else if(rule.compareCriteria() == RRT::dontContain)
            {
                for(QString kWord : rule.keyWords())
                    if(model->suffix().contains(kWord))
                        condition = true;
                if(!condition)
                    filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
            }
            else if(rule.compareCriteria() == RRT::match)
            {
                for(QString kWord : rule.keyWords())
                    if(model->suffix() == kWord)
                    {
                        filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
                        break;
                    }
            }
            else if(rule.compareCriteria() == RRT::dontMatch)
            {
                for(QString kWord : rule.keyWords())
                    if(model->suffix() == kWord)
                        condition = true;
                if(!condition)
                    filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
            }
        }
        /*
         * Check if parent folder name meets the criterias set
         */

        else if(rule.criteria() == RRT::fileParentMode)
        {
            if(rule.compareCriteria() == RRT::contains)
            {
                for(QString kWord : rule.keyWords())
                    if(static_cast<FileModel*>(model->_parent)->fileName().contains(kWord))
                        condition = true;

                if(condition)
                    filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
            }
            else if(rule.compareCriteria() == RRT::dontContain)
            {
                for(QString kWord : rule.keyWords())
                    if(static_cast<FileModel*>(model->_parent)->fileName().contains(kWord))
                        condition = true;
                if(!condition)
                    filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
            }
            else if(rule.compareCriteria() == RRT::match)
            {
                for(QString kWord : rule.keyWords())
                {
                    if(static_cast<FileModel*>(model->_parent)->fileName() == kWord)
                        condition = true;
                }
                if(condition)
                    filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
            }
            else if(rule.compareCriteria() == RRT::dontMatch)
            {
                for(QString kWord : rule.keyWords())
                {
                    if(static_cast<FileModel*>(model->_parent)->fileName() == kWord)
                        condition = true;
                }
                if(!condition)
                    filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
            }
        }
        else if(rule.criteria() == RRT::fileSize)
        {
            if(rule.compareCriteria() != RRT::interval)
            {
                if(rule.compareCriteria() == RRT::lesserThan && model->size() < SBC::convertToBytes(rule.sizeLimit().first,rule.sizeLimit().second))
                    filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
                else if(rule.compareCriteria() == RRT::lesserOrEqualThan &&
                        model->size() <= SBC::convertToBytes(rule.sizeLimit().first,rule.sizeLimit().second))
                    filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
                else if(rule.compareCriteria() == RRT::equal &&
                        model->size() == SBC::convertToBytes(rule.sizeLimit().first,rule.sizeLimit().second))
                    filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
                else if(rule.compareCriteria() == RRT::greaterOrEqualThan &&
                        model->size() >= SBC::convertToBytes(rule.sizeLimit().first,rule.sizeLimit().second))
                    filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
                else if(rule.compareCriteria() == RRT::greaterThan &&
                        model->size() > SBC::convertToBytes(rule.sizeLimit().first,rule.sizeLimit().second))
                    filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
            }
            else if(rule.compareCriteria() == RRT::interval &&
                    model->size() >= SBC::convertToBytes(rule.sizeInterval().first.first,rule.sizeInterval().first.second) &&
                    model->size() <= SBC::convertToBytes(rule.sizeInterval().second.first,rule.sizeInterval().second.second))
                filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
        }
        else if(rule.criteria() == RRT::fileCreatedMode)
        {
            if(rule.compareCriteria() == RRT::interval)
            {
                if(rule.dateIntervals().first > model->birthTime() && rule.dateIntervals().second < model->birthTime())
                    filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
            }
            else if(rule.compareCriteria() != RRT::interval)
            {
                if(rule.compareCriteria() == RRT::youngerThan && rule.date() > model->birthTime())
                    filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
                else if(rule.compareCriteria() == RRT::exactDate && rule.date() == model->birthTime())
                    filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
                else if(rule.compareCriteria() == RRT::olderThan && rule.date() < model->birthTime())
                    filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
            }

        }
        else if(rule.criteria() == RRT::fileModifiedMode)
        {
            if(rule.compareCriteria() == RRT::interval)
            {
                if(rule.dateIntervals().first > model->lastModified() && rule.dateIntervals().second < model->lastModified())
                    filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
            }
            else if(rule.compareCriteria() != RRT::interval)
            {
                if(rule.compareCriteria() == RRT::youngerThan && rule.date() > model->lastModified())
                    filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
                else if(rule.compareCriteria() == RRT::exactDate && rule.date() == model->lastModified())
                    filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
                else if(rule.compareCriteria() == RRT::olderThan && rule.date() < model->lastModified())
                    filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
            }
        }
        else if(rule.criteria() == RRT::nonConditionalMode)
        {
            filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
        }
    }
    return filesToProcess;
}

FileObjectList FileWorker::generateFileObjects(const QStringList &paths, const QString &rPath, const RRT::FileTypeEntity &filter)
{
    FileObjectList resultingList;

    QStringList rPaths;
    if(rPath != QString() && rPath != "Alle")
        rPaths = QStringList(rPath);
    else
        rPaths = paths;

    for (QString path : rPaths)
    {
        QDirIterator it(path,QDir::AllEntries | QDir::NoDotAndDotDot | QDir::System | QDir::Hidden);
        while(it.hasNext())
        {
            auto modelDelegate = FileModelDelegate<FileModel>::buildDelegate(it.next());
            auto model = modelDelegate->model();
            FileModel parentModel;
            parentModel.setFile(path);
            modelDelegate->setParentModelDelegate(FileModelDelegate<FileModel>::buildDelegate(path));
            if(model->isFile() && filter == RRT::File)
                resultingList << modelDelegate;
            else if(model->isDir())
            {
                FileObjectList list = generateFileObjects(paths,model->absoluteFilePath());
                if(filter == RRT::Folder)
                {
                    modelDelegate->setChildren(list);
                    resultingList << modelDelegate;
                }
                else
                {
                    resultingList << list;
                }
            }
        }
    }
    return resultingList;
}


void FileWorker::countFolderItems(const QString &path, const QDir::Filters &filters, const QDirIterator::IteratorFlags &flags)
{
    if(isBusy)
    {
        emit infoReport(busyMessage);
        return;
    }
    isBusy = true;
    long taeller = 0;
    QDirIterator iT(path,filters,flags);
    while(iT.hasNext())
    {
        QFileInfo fil = iT.next();
        if(!fil.isDir())
            taeller++;
    }
    emit fileCount(taeller);
    isBusy = false;
}

void FileWorker::countFolders(const QStringList &Path)
{
    long taeller = 0;
    for (QString p : Path)
    {
         taeller += folderCount(p);
    }
    emit fileCount(taeller);
}

void FileWorker::handleProcessRequest()
{
    if(isBusy)
        return;
    else
        emit requestNextEntity();

}

void FileWorker::processFileInformationEntity(const IModelDelegate<FileInformationEntity,EntityType> *delegate)
{
    auto entity = delegate->model();

    QList<DirectoryItem> directories;
    for (auto path : entity->filePaths)
    {
        DirectoryItem item;

        QString denotation;
        item.path = path;
        double directorySize = SBC::convertFromBytes(folderSize(path),denotation);
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
        double directorySize = SBC::convertFromBytes(folderSize(p),denotation);
        item.dirSize = QString::number(directorySize) + " " + denotation;
        item.numberOfDirectories = folderCount(p);
        item.numberOfFiles = fileCountFromPath(p);
        item.directoryItemModels = ByteCollection::assembleItemModelsFromPath(p);
        item.sufList = ByteCollection::getListOfSuffixOccuriencies(p);
        directories << item;
    }
    emit processFinished(directories);
}

void FileWorker::processFileEntity(const IModelDelegate<FileRuleEntity,EntityType> *delegate)
{
    auto model = delegate->model();

    if(model->fileActionRule == RRT::Delete || model->fileActionRule == RRT::none)
    {
        // TODO: You have to pass an error related stringlist in order to be able to display errors
        removeFileItems(model->allFiles);
        auto entity = DelegateBuilder::buildFileInformationEntity
                <FileInformationEntity>(model->directoryPaths)->model();

        processFileInformationEntity(DelegateBuilder::buildDelegate<FileInformationEntity>(entity));
    }
    else if(model->fileActionRule == RRT::Move)
    {
        moveFileItems(model->allFiles,model->fileDestinations);
        auto entity = DelegateBuilder::buildFileInformationEntity
                <FileInformationEntity>(model->directoryPaths)->model();

        processFileInformationEntity(DelegateBuilder::buildDelegate<FileInformationEntity>(entity));

    }
    else if(model->fileActionRule == RRT::Copy)
        copyFileItems(model->allFiles,model->fileDestinations);

    model = nullptr;
}
