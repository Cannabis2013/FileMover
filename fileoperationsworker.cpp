#include "fileoperationsworker.h"

FileOperationsWorker::FileOperationsWorker()
{
    busyMessage = "Luke Fileworker is busy. Patience my young padawan.";
    isBusy = false;
    qt_ntfs_permission_lookup++;
}

bool FileOperationsWorker::removeFileItems(const QFileInfoList filePaths)
{
    if(filePaths.isEmpty())
        return false;

    QStringList errs;

    for(QFileInfo fItem: filePaths)
    {
        QString absoluteFilePath = fItem.absoluteFilePath();
        if(fItem.isFile())
        {
            QFile fileItem(fItem.absoluteFilePath());
            if(!fileItem.remove())
            {
                errs << "Operation on: " + fItem.fileName() + " in: " +
                    fItem.absolutePath() + " returned: " + fileItem.errorString();
            }
        }
        else if(fItem.isDir())
        {
            removeDir(absoluteFilePath,errs);
        }
    }

    if(!errs.isEmpty())
    {
        //Do some error handling here!
    }
    return true;
}

bool FileOperationsWorker::moveFileItems(const QFileInfoList files, const QStringList destinations)
{
    bool result = true;
    for(QString destPath : destinations)
    {
        for(QFileInfo file : files)
        {
            if(!moveRecursively(file.filePath(),destPath))
                result = false;
            else
                result = false;
        }
    }
    return result;
}

bool FileOperationsWorker::copyFileItems(const QFileInfoList files, const QStringList destinations)
{
    bool result = true;
    for(QString destPath : destinations)
    {
        for(QFileInfo file : files)
        {
            if(!copyRecursively(file.filePath(),destPath))
                result = false;
            else
                result = false;
        }
    }
    return result;
}

void FileOperationsWorker::processDirectoryCountEntity(EntityModel *entity)
{
    DirectoryCountEntity *fObject = static_cast<DirectoryCountEntity*>(entity);
    QFileInfo fInfo = fObject->directoryPath();

    fObject->setContentSize(folderSize(fInfo.absoluteFilePath()));

    emit sendFolderSizeEntity(fObject);
}

void FileOperationsWorker::removeDir(QString &dirName, QStringList &errs)
{
    QDir dir(dirName);

    QDirIterator it(dirName,QDir::NoDotAndDotDot |
                    QDir::System |
                    QDir::Hidden |
                    QDir::AllEntries,
                    QDirIterator::Subdirectories);
    while(it.hasNext())
    {
        QFileInfo fileObject = it.next();
        if(fileObject.isFile())
        {
            QFile file(fileObject.absoluteFilePath());
            if(!file.remove())
            {
                errs << "Operation on: " + file.fileName() + " in: " +
                    fileObject.absolutePath() + " returned: " + file.errorString();
            }
        }
        else if(fileObject.isDir())
        {
            QString filePath = fileObject.absoluteFilePath();
            removeDir(filePath,errs);
        }
    }
    dir.rmdir(dirName);
}

bool FileOperationsWorker::moveRecursively(QString path, QString destination)
{
    bool result = true;
    QDirIterator iT(path,QDir::NoDotAndDotDot | QDir::AllEntries);
    QDir dir(destination);
    if(dir.exists())
        dir.mkdir(destination);
    while(iT.hasNext())
    {
        bool success = false;
        QFileInfo fileInfo = iT.next();
        QString entryName = fileInfo.fileName();
        QString tempDest = checkAndCorrectForBackslash(destination) + entryName;
        if(fileInfo.isDir())
        {
            QDir iDir = fileInfo.absoluteDir();
            if(moveRecursively(fileInfo.absoluteFilePath(),tempDest))
            {
                iDir.removeRecursively();
                success = true;
            }
            result = success ? result : success;
        }
        else if(fileInfo.isFile())
        {
            if(QFile::copy(fileInfo.absoluteFilePath(),tempDest))
            {
                if(QFile::remove(fileInfo.absoluteFilePath()))
                    success = true;
            }
        }
        result = success ? result : success;
    }
    return result;
}

bool FileOperationsWorker::copyRecursively(QString path, QString destination)
{
    bool result = true;
    QDirIterator iT(path,QDir::NoDotAndDotDot | QDir::AllEntries);
    QDir dir(destination);
    if(dir.exists())
        dir.mkdir(destination);
    while(iT.hasNext())
    {
        QFileInfo fileInfo = iT.next();
        QString entryName = fileInfo.fileName();
        QString tempDest = checkAndCorrectForBackslash(destination) + entryName;
        if(fileInfo.isDir())
            result = copyRecursively(fileInfo.absoluteFilePath(),tempDest) ? result : false;
        else if(fileInfo.isFile())
            result =QFile::copy(fileInfo.absoluteFilePath(),tempDest) ? result : false;
    }
    return result;
}

void FileOperationsWorker::processEntity(EntityModel *entity)
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

int FileOperationsWorker::folderCount(QString p)
{
    QFileInfo info = p;
    QDirIterator ite(info.absoluteFilePath(),
                     QDir::NoDotAndDotDot |
                     QDir::NoDot | QDir::AllEntries,
                     QDirIterator::Subdirectories);
    int taeller = 0;
    while(ite.hasNext())
    {
        QFileInfo i = ite.fileInfo();
        taeller = i.isDir() ? ++taeller : taeller;
        emit sendStatusLineMessage(QString("%1 folders").arg(QString::number(taeller)));
        ite.next();
    }
    return taeller;
}

int FileOperationsWorker::fileCount(QString p)
{
    QFileInfo info = p;
    QDirIterator ite(info.absoluteFilePath(),
                     QDir::NoDotAndDotDot |
                     QDir::NoDot | QDir::Files,
                     QDirIterator::Subdirectories);
    int taeller = 0;
    while(ite.hasNext())
    {
        QFileInfo i = ite.next();
        taeller = i.isFile() ? ++taeller : taeller;
        emit sendStatusLineMessage(QString("%1 files").arg(QString::number(taeller)));
    }
    return taeller;
}

QList<QPair<QString, int> > FileOperationsWorker::getListOfSuffixOccuriencies(QString p)
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
        QFileInfo i = ite.next();;
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

long long FileOperationsWorker::folderSize(QString pf)
{
    long long sZ = 0;

    QFileInfo p = pf;
    QDirIterator iT(p.absoluteFilePath(),
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

QTreeWidgetItem *FileOperationsWorker::assembleItemModelsFromPath(QString p)
{
    QFileInfo info(p);
    if(!info.isDir() && !info.isFile())
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

QFileInfoList FileOperationsWorker::assembleEntryList(QString path)
{
    if(path == QString())
        return QFileInfoList();

    QDir directory(path);

    return directory.entryInfoList(QDir::AllEntries | QDir::System | QDir::Hidden);
}

QStringList FileOperationsWorker::createHeader(QFileInfo fi)
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

QFileInfoList FileOperationsWorker::processList(QFileInfoList files, SubRule rule)
{
    bool condition = false;

    QFileInfoList filesToProcess;
    for(QFileInfo file : files)
    {
        // Evaluating filename patterns
        if(rule.fieldCondition == rD::filepathMode)
        {
            if(rule.fileCompareMode == rD::contains)
            {
                for(QString kWord : rule.keyWords)
                    if(file.fileName().contains(kWord))
                        condition = true;
                if(condition)
                    filesToProcess << file;
            }
            else if(rule.fileCompareMode == rD::dontContain)
            {
                for(QString kWord : rule.keyWords)
                    if(file.fileName().contains(kWord))
                        condition = true;
                if(!condition)
                    filesToProcess << file;
            }
            else if(rule.fileCompareMode == rD::match)
            {
                for(QString kWord : rule.keyWords)
                    if(file.fileName() == kWord)
                        condition = true;
                if(condition)
                    filesToProcess << file;
            }
            else if(rule.fileCompareMode == rD::dontMatch)
            {
                for(QString kWord : rule.keyWords)
                    if(file.fileName() == kWord)
                        condition = true;
                if(!condition)
                    filesToProcess << file;
            }
        }

        // Evaluating file extension related patterns
        else if(rule.fieldCondition == rD::extensionMode && file.isFile())
        {
            if(rule.fileCompareMode == rD::contains)
            {
                for(QString kWord : rule.keyWords)
                    if(file.suffix().contains(kWord))
                        condition = true;
                if(condition)
                    filesToProcess << file;
            }
            else if(rule.fileCompareMode == rD::dontContain)
            {
                for(QString kWord : rule.keyWords)
                    if(file.suffix().contains(kWord))
                        condition = true;
                if(!condition)
                    filesToProcess << file;
            }
            else if(rule.fileCompareMode == rD::match)
            {
                for(QString kWord : rule.keyWords)
                    if(file.suffix() == kWord)
                        condition = true;
                if(condition)
                    filesToProcess << file;
            }
            else if(rule.fileCompareMode == rD::dontMatch)
            {
                for(QString kWord : rule.keyWords)
                    if(file.suffix() == kWord)
                        condition = true;
                if(!condition)
                    filesToProcess << file;
            }
        }
        else if(rule.fieldCondition == rD::sizeMode)
        {
            if(rule.fileCompareMode != rD::interval)
            {
                if(rule.fileCompareMode == rD::lesser && file.size() < fW::byteConvert(rule.sizeLimit.first,rule.sizeLimit.second))
                    filesToProcess << file;
                else if(rule.fileCompareMode == rD::lesserOrEqual &&
                        file.size() <= fW::byteConvert(rule.sizeLimit.first,rule.sizeLimit.second))
                    filesToProcess << file;
                else if(rule.fileCompareMode == rD::equal &&
                        file.size() == fW::byteConvert(rule.sizeLimit.first,rule.sizeLimit.second))
                    filesToProcess << file;
                else if(rule.fileCompareMode == rD::biggerOrEqual &&
                        file.size() >= fW::byteConvert(rule.sizeLimit.first,rule.sizeLimit.second))
                    filesToProcess << file;
                else if(rule.fileCompareMode == rD::bigger &&
                        file.size() > fW::byteConvert(rule.sizeLimit.first,rule.sizeLimit.second))
                    filesToProcess << file;
            }
            else if(rule.fileCompareMode == rD::interval &&
                    file.size() >= fW::byteConvert(rule.sizeIntervalLimits.first.first,rule.sizeIntervalLimits.first.second) &&
                    file.size() <= fW::byteConvert(rule.sizeIntervalLimits.second.first,rule.sizeIntervalLimits.second.second))
                filesToProcess << file;

        }
        else if(rule.fieldCondition == rD::dateCreatedMode)
        {
            if(rule.fileCompareMode == rD::interval)
            {
                if(rule.intervalDate.first > file.created() && rule.intervalDate.second < file.created())
                    filesToProcess << file;
            }
            else if(rule.fileCompareMode != rD::interval)
            {
                if(rule.fileCompareMode == rD::youngerThan && rule.fixedDate.second > file.created())
                    filesToProcess << file;
                else if(rule.fileCompareMode == rD::exactDate && rule.fixedDate.second== file.created())
                    filesToProcess << file;
                else if(rule.fileCompareMode == rD::olderThan && rule.fixedDate.second < file.created())
                    filesToProcess << file;
            }

        }
        else if(rule.fieldCondition == rD::dateModifiedMode)
        {
            if(rule.fileCompareMode == rD::interval)
            {
                if(rule.intervalDate.first > file.lastModified() && rule.intervalDate.second < file.lastModified())
                    filesToProcess << file;
            }
            else if(rule.fileCompareMode != rD::interval)
            {
                if(rule.fileCompareMode == rD::youngerThan && rule.fixedDate.second > file.lastModified())
                    filesToProcess << file;
                else if(rule.fileCompareMode == rD::exactDate && rule.fixedDate.second == file.lastModified())
                    filesToProcess << file;
                else if(rule.fileCompareMode == rD::olderThan && rule.fixedDate.second < file.lastModified())
                    filesToProcess << file;
            }
        }
        else if(rule.fieldCondition == rD::nonConditionalMode)
        {
            filesToProcess << file;
        }
    }
    return filesToProcess;
}

QFileInfoList FileOperationsWorker::generateFilesList(QString rPath, QStringList paths, bool recursive)
{
    QStringList rPaths;
    if(rPath != QString() && rPath != "Alle")
        rPaths = QStringList(rPath);
    else
        rPaths = paths;

    QFileInfoList allFiles;
    if(!recursive)
    /* Add items to a "QFileInfoList" in a non-recursive manner,
     * which means that directories, excluding their content, is added to the list of QFileInfo objects.
     */
    {
        for(QString path : rPaths)
        {
            QDir dirContent(path);
            allFiles += dirContent.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::System);
        }
    }
    // Add QFileInfo objects to a QFileInfoList in a recursive manner, but excludes directories.
    else
    {
        for(QString path : paths)
        {
            QDirIterator it(path,
                            QDir::AllEntries | QDir::NoDotAndDotDot | QDir::System,
                            QDirIterator::Subdirectories);
            while(it.hasNext())
            {
                QFileInfo fileItem = it.next();
                if(!fileItem.isDir())
                    allFiles.append(fileItem);
            }
        }
    }
    return allFiles;
}

void FileOperationsWorker::countNumberOfFolderItems(QString path, QDir::Filters f, QDirIterator::IteratorFlags i)
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

void FileOperationsWorker::countFolders(QStringList Path)
{
    long taeller = 0;
    for (QString p : Path)
    {
         taeller += folderCount(p);
    }
    emit fileCount(taeller);
}

void FileOperationsWorker::handleProcessRequest()
{
    if(isBusy)
        return;
    else
        emit requestNextEntity();
}

void FileOperationsWorker::processFileInformationEntity(EntityModel * entity)
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
        item.directoryContent = assembleEntryList(path);
        item.directoryItemModels = assembleItemModelsFromPath(path);
        item.sufList = getListOfSuffixOccuriencies(path);
        directories << item;
    }

    emit processFinished(directories);
}

void FileOperationsWorker::reProcessFileInformationEntity(const QStringList paths)
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

void FileOperationsWorker::processFileActionEntity(EntityModel *entity)
{
    FileActionEntity *item = static_cast<FileActionEntity*>(entity);
    if(item->fileActionRule() == rD::Delete || item->fileActionRule() == rD::none)
    {
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

