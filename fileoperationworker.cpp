#include "fileoperationworker.h"

FileOperationWorker::FileOperationWorker()
{
    busyMessage = "Luke Fileworker is busy. Patience my young padawan.";
    isBusy = false;
    #ifdef defined (__WIN32__ || __WIN64__)
        qt_ntfs_permission_lookup++;
    #endif
}

bool FileOperationWorker::removeFileItems(const FileObjectList filePaths, QStringList *const err)
{
    if(filePaths.isEmpty())
        return true;

    for(FileObject* fObject: filePaths)
    {
        QString absoluteFilePath = fObject->absoluteFilePath();
        if(fObject->isFile())
        {
            QFile fileItem(fObject->absoluteFilePath());
            if(!fileItem.remove())
            {
                *err << "Operation on: " + fObject->fileName() + " in: " +
                    fObject->absolutePath() + " returned: " + fileItem.errorString();
            }
        }
        else if(fObject->isDir())
        {
            removeFileItems(fObject->children(),err);
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

        for(FileObject* fObject : fileObjects)
        {
            bool noErrors = true;
            QString AbsoluteFilePath = checkAndCorrectForBackslash(destPath) + fObject->fileName();
            if(fObject->isDir())
            {
                noErrors = moveFileItems(fObject->children(),QStringList() << AbsoluteFilePath);
                result = noErrors ? result : false;
            }
            else if(fObject->isFile())
            {
                noErrors = QFile::copy(fObject->absoluteFilePath(),AbsoluteFilePath);
                result = noErrors ? result : false;
            }
            else if(!fObject->exists())
                continue;
            if(noErrors)
                removeFileItems(FileObjectList() << fObject);
        }
    }
    return result;
}

bool FileOperationWorker::copyFileItems(const FileObjectList fileObjects, const QStringList destinations, QStringList * const err)
{
    bool result = true;
    for(QString destPath : destinations)
    {
        QDir dir(destPath);
        if(!dir.exists())
            dir.mkdir(destPath);

        for(FileObject* fObject : fileObjects)
        {
            bool noErrors = true;
            QString AbsoluteFilePath = checkAndCorrectForBackslash(destPath) + fObject->fileName();
            if(fObject->isDir())
            {
                noErrors = copyFileItems(fObject->children(),QStringList() << AbsoluteFilePath);
                result = noErrors ? result : false;
            }
            else if(fObject->isFile())
            {
                noErrors = QFile::copy(fObject->absoluteFilePath(),AbsoluteFilePath);
                result = noErrors ? result : false;
            }
            else if(!fObject->exists())
                continue;
        }
    }
    return result;
}

void FileOperationWorker::processDirectoryCountEntity(EntityModel *entity)
{
    DirectoryCountEntity *fObject = static_cast<DirectoryCountEntity*>(entity);
    QFileInfo fInfo = fObject->directoryPath();

    fObject->setContentSize(folderSize(fInfo.absoluteFilePath()));

    emit sendFolderSizeEntity(fObject);
}

void FileOperationWorker::processEntity(EntityModel *entity)
{
    isBusy = true;
    if(entity->entityType() == entityType::nullAction)
    {
        emit jobDone(true);
        isBusy = false;
        return;
    }

    if(entity->entityType() == entityType::FileAction)
        processFileActionEntity(entity);
    else if(entity->entityType() == entityType::informationAction)
        processFileInformationEntity(entity);
    else if(entity->entityType() == entityType::directoryCountAction)
        processDirectoryCountEntity(entity);

    entity = nullptr;
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
        headers << directoryName(fi.absoluteFilePath());
        headers << fi.absoluteFilePath();
        headers << "";
        headers << "Mappe";
        if(sz <=mp)
            headers << QString::number(fi.size()) + " b";
        else if(sz > mp && sz <mp*mp)
            headers << QString::number(sz/mp) + " kb";
        else if(sz >= (mp*mp) && sz <(mp*mp*mp))
            headers << QString::number(sz/(mp*mp)) + " mb";
        else
            headers << QString::number(sz/(mp*mp*mp)) + " gb";
        headers << fi.lastModified().toString();
        headers << fi.lastRead().toString();
    }
    else if(fi.isFile())
    {
        headers << fi.fileName();
        headers << fi.absoluteFilePath();
        headers << fi.suffix();
        headers << "Fil";
        if(fi.size() <=mp)
            headers << QString::number(fi.size()) + " b";
        else if(fi.size() > mp && fi.size() <mp*mp)
            headers << QString::number(fi.size()/mp) + " kb";
        else if(fi.size() >= (mp*mp) && fi.size() <(mp*mp*mp))
            headers << QString::number(fi.size()/(mp*mp)) + " mb";
        else
            headers << QString::number(fi.size()/(mp*mp*mp)) + " gb";
        headers << fi.lastModified().toString();
        headers << fi.lastRead().toString();
    }
    else
    {

        headers << "Filename" << "Filepath" << "Suffix"<< "Type" << "Size" << "Last modified" << "Last read";
    }
    return headers;
}

FileObjectList FileOperationWorker::processFileObjects(FileObjectList fileObjects, SubRule rule)
{
    FileObjectList filesToProcess;
    for(FileObject* fObject : fileObjects)
    {
        bool condition = false;
        // Evaluating filename patterns
        if(rule.fieldCondition == rD::filepathMode)
        {
            if(rule.fileCompareMode == rD::contains)
            {
                for(QString kWord : rule.keyWords)
                    if(fObject->fileName().contains(kWord))
                        condition = true;
                if(condition)
                    filesToProcess << fObject;
            }
            else if(rule.fileCompareMode == rD::dontContain)
            {
                for(QString kWord : rule.keyWords)
                    if(fObject->fileName().contains(kWord))
                        condition = true;
                if(!condition)
                    filesToProcess << fObject;
            }
            else if(rule.fileCompareMode == rD::match)
            {
                for(QString kWord : rule.keyWords)
                    if(fObject->fileName() == kWord)
                        condition = true;
                if(condition)
                    filesToProcess << fObject;
            }
            else if(rule.fileCompareMode == rD::dontMatch)
            {
                for(QString kWord : rule.keyWords)
                    if(fObject->fileName() == kWord)
                        condition = true;
                if(!condition)
                    filesToProcess << fObject;
            }
        }

        // Evaluating file extension related patterns
        else if(rule.fieldCondition == rD::extensionMode && fObject->isFile())
        {
            if(rule.fileCompareMode == rD::contains)
            {
                for(QString kWord : rule.keyWords)
                    if(fObject->suffix().contains(kWord))
                        condition = true;

                if(condition)
                    filesToProcess << fObject;
            }
            else if(rule.fileCompareMode == rD::dontContain)
            {
                for(QString kWord : rule.keyWords)
                    if(fObject->suffix().contains(kWord))
                        condition = true;
                if(!condition)
                    filesToProcess << fObject;
            }
            else if(rule.fileCompareMode == rD::match)
            {
                for(QString kWord : rule.keyWords)
                    if(fObject->suffix() == kWord)
                        condition = true;
                if(condition)
                    filesToProcess << fObject;
            }
            else if(rule.fileCompareMode == rD::dontMatch)
            {
                for(QString kWord : rule.keyWords)
                    if(fObject->suffix() == kWord)
                        condition = true;
                if(!condition)
                    filesToProcess << fObject;
            }
        }
        /*
         * Check if parent folder name meets the criterias set
         */

        else if(rule.fieldCondition == rD::parentFolderMode)
        {
            if(rule.fileCompareMode == rD::contains)
            {
                for(QString kWord : rule.keyWords)
                    if(fObject->parentFolderObject()->fileName().contains(kWord))
                        condition = true;

                if(condition)
                    filesToProcess << fObject;
            }
            else if(rule.fileCompareMode == rD::dontContain)
            {
                for(QString kWord : rule.keyWords)
                    if(fObject->parentFolderObject()->fileName().contains(kWord))
                        condition = true;
                if(!condition)
                    filesToProcess << fObject;
            }
            else if(rule.fileCompareMode == rD::match)
            {
                for(QString kWord : rule.keyWords)
                    if(fObject->parentFolderObject()->fileName() == kWord)
                        condition = true;
                if(condition)
                    filesToProcess << fObject;
            }
            else if(rule.fileCompareMode == rD::dontMatch)
            {
                for(QString kWord : rule.keyWords)
                    if(fObject->parentFolderObject()->fileName() == kWord)
                        condition = true;
                if(!condition)
                    filesToProcess << fObject;
            }
        }
        else if(rule.fieldCondition == rD::sizeMode)
        {
            if(rule.fileCompareMode != rD::interval)
            {
                if(rule.fileCompareMode == rD::lesser && fObject->size() < fW::byteConvert(rule.sizeLimit.first,rule.sizeLimit.second))
                    filesToProcess << fObject;
                else if(rule.fileCompareMode == rD::lesserOrEqual &&
                        fObject->size() <= fW::byteConvert(rule.sizeLimit.first,rule.sizeLimit.second))
                    filesToProcess << fObject;
                else if(rule.fileCompareMode == rD::equal &&
                        fObject->size() == fW::byteConvert(rule.sizeLimit.first,rule.sizeLimit.second))
                    filesToProcess << fObject;
                else if(rule.fileCompareMode == rD::biggerOrEqual &&
                        fObject->size() >= fW::byteConvert(rule.sizeLimit.first,rule.sizeLimit.second))
                    filesToProcess << fObject;
                else if(rule.fileCompareMode == rD::bigger &&
                        fObject->size() > fW::byteConvert(rule.sizeLimit.first,rule.sizeLimit.second))
                    filesToProcess << fObject;
            }
            else if(rule.fileCompareMode == rD::interval &&
                    fObject->size() >= fW::byteConvert(rule.sizeIntervalLimits.first.first,rule.sizeIntervalLimits.first.second) &&
                    fObject->size() <= fW::byteConvert(rule.sizeIntervalLimits.second.first,rule.sizeIntervalLimits.second.second))
                filesToProcess << fObject;
        }
        else if(rule.fieldCondition == rD::dateCreatedMode)
        {
            if(rule.fileCompareMode == rD::interval)
            {
                if(rule.intervalDate.first > fObject->birthTime() && rule.intervalDate.second < fObject->birthTime())
                    filesToProcess << fObject;
            }
            else if(rule.fileCompareMode != rD::interval)
            {
                if(rule.fileCompareMode == rD::youngerThan && rule.fixedDate.second > fObject->birthTime())
                    filesToProcess << fObject;
                else if(rule.fileCompareMode == rD::exactDate && rule.fixedDate.second== fObject->birthTime())
                    filesToProcess << fObject;
                else if(rule.fileCompareMode == rD::olderThan && rule.fixedDate.second < fObject->birthTime())
                    filesToProcess << fObject;
            }

        }
        else if(rule.fieldCondition == rD::dateModifiedMode)
        {
            if(rule.fileCompareMode == rD::interval)
            {
                if(rule.intervalDate.first > fObject->lastModified() && rule.intervalDate.second < fObject->lastModified())
                    filesToProcess << fObject;
            }
            else if(rule.fileCompareMode != rD::interval)
            {
                if(rule.fileCompareMode == rD::youngerThan && rule.fixedDate.second > fObject->lastModified())
                    filesToProcess << fObject;
                else if(rule.fileCompareMode == rD::exactDate && rule.fixedDate.second == fObject->lastModified())
                    filesToProcess << fObject;
                else if(rule.fileCompareMode == rD::olderThan && rule.fixedDate.second < fObject->lastModified())
                    filesToProcess << fObject;
            }
        }
        else if(rule.fieldCondition == rD::nonConditionalMode)
        {
            filesToProcess << fObject;
        }
    }
    return filesToProcess;
}

FileObjectList FileOperationWorker::generateFileObjects(const QStringList &paths, const QString &rPath, ruleDefinitions::fileTypeEntity filter)
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
            FileObject* fObject = new FileObject(it.next());
            fObject->setParentFolderObject(new FileObject(path));
            if(fObject->isFile() && filter == rD::File)
                resultingList << fObject;
            else if(fObject->isDir())
            {
                FileObjectList list = generateFileObjects(paths,fObject->absoluteFilePath());
                if(filter == rD::Folder)
                {
                    fObject->setChildren(list);
                    resultingList << fObject;
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

void FileOperationWorker::processFileInformationEntity(EntityModel * entity)
{
    fileInformationEntity *itemModel = static_cast<fileInformationEntity*>(entity);

    QList<DirectoryItem> directories;
    for (int i = 0;i <itemModel->pathCount();i++)
    {
        DirectoryItem item;
        QString path = itemModel->pathAt(i);
        QString denotation;
        item.path = path;
        double directorySize = convertSizeToAppropriateUnits(folderSize(path),denotation);
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

void FileOperationWorker::reProcessFileInformationEntity(const QStringList paths)
{
    QList<DirectoryItem>directories;
    for(QString p: paths)
    {
        QString denotation;
        DirectoryItem item;
        item.path = p;
        double directorySize = convertSizeToAppropriateUnits(folderSize(p),denotation);
        item.dirSize = QString::number(directorySize) + " " + denotation;
        item.numberOfDirectories = folderCount(p);
        item.numberOfFiles = fileCount(p);
        item.directoryItemModels = assembleItemModelsFromPath(p);
        item.sufList = getListOfSuffixOccuriencies(p);
        directories << item;
    }
    emit processFinished(directories);
}

void FileOperationWorker::processFileActionEntity(EntityModel *entity)
{
    FileActionEntity *item = static_cast<FileActionEntity*>(entity);
    if(item->fileActionRule() == rD::Delete || item->fileActionRule() == rD::none)
    {
        // TODO: You have to pass an error related stringlist in order to be able to display errors
        removeFileItems(item->directoryFileList());
        processFileInformationEntity(new fileInformationEntity(item->directoryPaths()));
    }
    else if(item->fileActionRule() == rD::Move)
    {
        moveFileItems(item->directoryFileList(),item->fileActionDestinations());
        processFileInformationEntity(new fileInformationEntity(item->directoryPaths()));
    }
    else if(item->fileActionRule() == rD::Copy)
        copyFileItems(item->directoryFileList(),item->fileActionDestinations());

    item = nullptr;
}
