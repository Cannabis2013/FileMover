#include "fileoperationworker.h"

FileOperationWorker::FileOperationWorker()
{
    busyMessage = "Luke Fileworker is busy. Patience my young padawan.";
    isBusy = false;
    #ifdef __WIN32__
        qt_ntfs_permission_lookup++;
    #elif __WIN64__
        qt_ntfs_permission_lookup++;
    #endif
}

bool FileOperationWorker::removeFileItems(const FileObjectList& filePaths, QStringList *const err)
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

bool FileOperationWorker::moveFileItems(const FileObjectList fileObjects, const QStringList destinations, QStringList * const err)
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
            QString AbsoluteFilePath = checkAndCorrectForBackslash(destPath) + model->fileName();
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

bool FileOperationWorker::copyFileItems(const FileObjectList fileObjects, const QStringList destinations, QStringList * const err)
{
    // TODO: Implement some error handling when something goes wrong
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
            QString AbsoluteFilePath = checkAndCorrectForBackslash(destPath) + model->fileName();
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

void FileOperationWorker::processDirectoryCountEntity(const IModelDelegate<DirectoryEntity, EntityType> *delegate)
{
    auto entity = delegate->modelValue();

    QFileInfo fInfo = entity->directoryPath;

    entity->directorySize = folderSize(fInfo.absoluteFilePath());

    emit sendFolderSizeEntity(entity);
}

void FileOperationWorker::processEntity(IModelDelegate<EntityModel,EntityType> *delegate)
{
    isBusy = true;
    if(delegate->type() == EntityModel::nullEntity)
    {
        emit jobDone(true);
        isBusy = false;
        return;
    }

    if(delegate->type() == EntityModel::fileActionEntity)
        processFileEntity(DelegateBuilder::buildDelegate<FileRuleEntity>(delegate->model()));
    else if(delegate->type() == EntityModel::fileInformationEntity)
        processFileInformationEntity(DelegateBuilder::buildDelegate<FileInformationEntity>(delegate->model()));
    else if(delegate->type() == EntityModel::directoryCountEntity)
        processDirectoryCountEntity(DelegateBuilder::buildDelegate<DirectoryEntity>(delegate->model()));

    delegate = nullptr;
    isBusy = false;

    emit requestNextEntity();
}

int FileOperationWorker::folderCount(QString p)
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

int FileOperationWorker::fileCount(QString p)
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

QList<QPair<QString, int> > FileOperationWorker::getListOfSuffixOccuriencies(QString p)
{
    QList<QPair<QString, int> > resultingList;
    QStringList allSufs,tempSufs;
    QDirIterator ite(p,
                     QDir::NoDotAndDotDot |
                     QDir::Files |
                     QDir::Hidden |
                     QDir::System,
                     QDirIterator::Subdirectories);
    while(ite.hasNext())
    {
        QFileInfo i = ite.next();
        if(i.isFile())
        {
            if(i.suffix() == "")
                allSufs << "Unknown";
            else
                allSufs << i.suffix();
        }
    }

    tempSufs = allSufs;
    foreach (QString suf, allSufs)
    {
        int taeller = 0;
        if(tempSufs.contains(suf))
        {
            taeller = tempSufs.count(suf);
            tempSufs.removeAll(suf);
            QPair<QString,int> sPar;
            sPar.first = suf;
            sPar.second = taeller;
            resultingList.append(sPar);
        }
    }
    return resultingList;
}

long long FileOperationWorker::folderSize(QString pf)
{
    long long sZ = 0;

    QDirIterator iT(QFileInfo(pf).absoluteFilePath(),
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

QTreeWidgetItem *FileOperationWorker::assembleItemModelsFromPath(QString p)
{
    if(!QFileInfo(p).isDir() && !QFileInfo(p).exists())
        return nullptr;

    QTreeWidgetItem *result = new QTreeWidgetItem(createHeader(p));
    QDirIterator ite(p,QDir::NoDotAndDotDot | QDir::AllEntries);
    while(ite.hasNext())
    {
        QFileInfo fInfo = ite.next();

        if(fInfo.isDir())
        {
            QTreeWidgetItem *dirItem = assembleItemModelsFromPath(fInfo.absoluteFilePath());
            result->addChild(dirItem);
        }
        else if(fInfo.isFile())
        {
            result->addChild(new QTreeWidgetItem(createHeader(fInfo)));
        }
    }
    return result;
}

QStringList FileOperationWorker::createHeader(QFileInfo fi)
{
    // Filename/dirname - Filepath - file extension - file type - Filesize - lastModified - Last read
    QStringList headers;
    int mp = 1024;

    if(fi.isDir())
    {
        double sz = 0;
        QDirIterator i(fi.filePath(),QDirIterator::Subdirectories);
        while(i.hasNext())
            sz += QFile(i.next()).size();

        headers << directoryName(fi.absoluteFilePath())
                << fi.absoluteFilePath()
                << ""
                << "Mappe";

        if(sz <=mp)
            headers << QString::number(fi.size()) + " b";
        else if(sz > mp && sz <mp*mp)
            headers << QString::number(sz/mp) + " kb";
        else if(sz >= (mp*mp) && sz <(mp*mp*mp))
            headers << QString::number(sz/(mp*mp)) + " mb";
        else
            headers << QString::number(sz/(mp*mp*mp)) + " gb";

        headers << fi.lastModified().toString()
                << fi.lastRead().toString();
    }
    else if(fi.isFile())
    {
        headers << fi.fileName()
                << fi.absoluteFilePath()
                << fi.suffix()
                << "Fil";

        if(fi.size() <=mp)
            headers << QString::number(fi.size()) + " b";
        else if(fi.size() > mp && fi.size() <mp*mp)
            headers << QString::number(fi.size()/mp) + " kb";
        else if(fi.size() >= (mp*mp) && fi.size() <(mp*mp*mp))
            headers << QString::number(fi.size()/(mp*mp)) + " mb";
        else
            headers << QString::number(fi.size()/(mp*mp*mp)) + " gb";

        headers << fi.lastModified().toString()
                << fi.lastRead().toString();
    }
    else
    {

        headers << "Filename"
                << "Filepath"
                << "Suffix"
                << "Type"
                << "Size"
                << "Last modified"
                << "Last read";
    }
    return headers;
}

FileObjectList FileOperationWorker::processFileObjects(FileObjectList fileObjects, SubRule rule)
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
                if(rule.compareCriteria() == RRT::lesserThan && model->size() < fW::convertToBytes(rule.sizeLimit().first,rule.sizeLimit().second))
                    filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
                else if(rule.compareCriteria() == RRT::lesserOrEqualThan &&
                        model->size() <= fW::convertToBytes(rule.sizeLimit().first,rule.sizeLimit().second))
                    filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
                else if(rule.compareCriteria() == RRT::equal &&
                        model->size() == fW::convertToBytes(rule.sizeLimit().first,rule.sizeLimit().second))
                    filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
                else if(rule.compareCriteria() == RRT::greaterOrEqualThan &&
                        model->size() >= fW::convertToBytes(rule.sizeLimit().first,rule.sizeLimit().second))
                    filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
                else if(rule.compareCriteria() == RRT::greaterThan &&
                        model->size() > fW::convertToBytes(rule.sizeLimit().first,rule.sizeLimit().second))
                    filesToProcess << FileModelDelegate<FileModel>::buildDelegate(model->absoluteFilePath());
            }
            else if(rule.compareCriteria() == RRT::interval &&
                    model->size() >= fW::convertToBytes(rule.sizeInterval().first.first,rule.sizeInterval().first.second) &&
                    model->size() <= fW::convertToBytes(rule.sizeInterval().second.first,rule.sizeInterval().second.second))
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

FileObjectList FileOperationWorker::generateFileObjects(const QStringList &paths, const QString &rPath, RRT::FileTypeEntity filter)
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


void FileOperationWorker::countNumberOfFolderItems(QString path, QDir::Filters f, QDirIterator::IteratorFlags i)
{
    if(isBusy)
    {
        emit infoReport(busyMessage);
        return;
    }
    isBusy = true;
    long taeller = 0;
    QDirIterator iT(path,f,i);
    while(iT.hasNext())
    {
        QFileInfo fil = iT.next();
        if(!fil.isDir())
            taeller++;
    }
    emit fileCount(taeller);
    isBusy = false;
}

void FileOperationWorker::countFolders(QStringList Path)
{
    long taeller = 0;
    for (QString p : Path)
    {
         taeller += folderCount(p);
    }
    emit fileCount(taeller);
}

void FileOperationWorker::handleProcessRequest()
{
    if(isBusy)
        return;
    else
        emit requestNextEntity();

}

void FileOperationWorker::processFileInformationEntity(const IModelDelegate<FileInformationEntity,EntityType> *delegate)
{
    auto entity = delegate->model();

    QList<DirectoryItem> directories;
    for (auto path : entity->filePaths)
    {
        DirectoryItem item;

        QString denotation;
        item.path = path;
        double directorySize = convertFromBytes(folderSize(path),denotation);
        item.dirSize = QString::number(directorySize) + " " + denotation;
        item.numberOfDirectories = folderCount(path);
        item.numberOfFiles = fileCount(path);
        item.directoryContent = QDir(path).entryInfoList(QDir::AllEntries |
                                                         QDir::System |
                                                         QDir::Hidden);
        item.directoryItemModels = assembleItemModelsFromPath(path);
        item.sufList = getListOfSuffixOccuriencies(path);
        directories << item;
    }

    emit processFinished(directories);
}

void FileOperationWorker::reProcessFileInformationEntity(const QStringList &paths)
{
    QList<DirectoryItem>directories;
    for(const QString &p : paths)
    {
        QString denotation;
        DirectoryItem item;
        item.path = p;
        double directorySize = convertFromBytes(folderSize(p),denotation);
        item.dirSize = QString::number(directorySize) + " " + denotation;
        item.numberOfDirectories = folderCount(p);
        item.numberOfFiles = fileCount(p);
        item.directoryItemModels = assembleItemModelsFromPath(p);
        item.sufList = getListOfSuffixOccuriencies(p);
        directories << item;
    }
    emit processFinished(directories);
}

void FileOperationWorker::processFileEntity(const IModelDelegate<FileRuleEntity,EntityType> *delegate)
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
