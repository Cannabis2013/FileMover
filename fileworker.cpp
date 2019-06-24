#include "fileworker.h"

FileWorker::FileWorker(EntityQueueManager *pRef)
{
    pControllerReference = pRef;
    busyMessage = "Luke Fileworker is busy. Patience my young padawan.";
    isBusy = false;
    qt_ntfs_permission_lookup++;
}

bool FileWorker::removeFileItems(const QFileInfoList filePaths)
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

bool FileWorker::moveEntities(const QFileInfoList files, const QStringList destinations)
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

bool FileWorker::copyEntities(const QFileInfoList files, const QStringList destinations)
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

DirectoryEntity FileWorker::folderContentSize(QString p)
{
    isBusy = true;

    QFileInfo fInfo = p;

    DirectoryEntity fObject;
    fObject.setDirectoryPath(fInfo.absoluteFilePath());
    fObject.setContentSize(folderSize(fInfo.absoluteFilePath()));
    fObject.setDirectoryName(fInfo.fileName());
    isBusy = false;
    return fObject;
}

QList<DirectoryEntity> FileWorker::foldersContentSize(QStringList l)
{
    QList<DirectoryEntity>resultingList;
    isBusy = true;

    for(QString path : l)
    {
        QFileInfo fInfo = path;
        DirectoryEntity fObject;
        fObject.setDirectoryPath(fInfo.absoluteFilePath());
        fObject.setContentSize(folderSize(fInfo.absoluteFilePath()));
        fObject.setDirectoryName(fInfo.fileName());
        resultingList << fObject;
    }
    isBusy = false;
    return resultingList;
}

void FileWorker::removeDir(QString &dirName, QStringList &errs)
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

bool FileWorker::moveRecursively(QString path, QString destination)
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

bool FileWorker::copyRecursively(QString path, QString destination)
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

void FileWorker::beginProcess()
{
    isBusy = true;
    bool isDone = true;
    while(!pControllerReference->fileActionEntityQueue())
    {
        FileActionEntity item = pControllerReference->takeFileActionEntity();
        if(item.fileActionRule() == rD::Delete || item.fileActionRule() == rD::none)
        {
            isDone = removeFileItems(item.directoryFileList()) ? isDone : false;
            reProcessFileInformations(item.directoryPaths());
        }
        else if(item.fileActionRule() == rD::Move)
            isDone = moveEntities(item.directoryFileList(),item.fileActionDestinations()) ? isDone : false;
        else if(item.fileActionRule() == rD::Copy)
            isDone = copyEntities(item.directoryFileList(),item.fileActionDestinations()) ? isDone : false;
    }
    isBusy = false;
    emit jobDone(isDone);
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
        QFileInfo i = ite.fileInfo();
        taeller = i.isDir() ? ++taeller : taeller;
        emit itemText(QString("%1 folders").arg(QString::number(taeller)));
        ite.next();
    }
    return taeller;
}

int FileWorker::fileCount(QString p)
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
        emit itemText(QString("%1 files").arg(QString::number(taeller)));
    }
    return taeller;
}

QList<QPair<QString, int> > FileWorker::getListOfSuffixOccuriencies(QString p)
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

long long FileWorker::folderSize(QString pf)
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
            emit itemText( "Calculating folder size: " + f.fileName());
         }
         f.close();
     }
    return sZ;
}

QTreeWidgetItem *FileWorker::scanDir(QString p)
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
            QTreeWidgetItem *dirItem = scanDir(fInfo.absoluteFilePath());
            result->addChild(dirItem);
        }
        else if(fInfo.isFile())
        {
            result->addChild(new QTreeWidgetItem(createHeader(fInfo)));
        }
    }
    return result;
}

QStringList FileWorker::createHeader(QFileInfo fi)
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

QFileInfoList FileWorker::processList(QFileInfoList files, SubRule rule)
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
        //
        else if(rule.fieldCondition == rD::typeMode)
        {
            if(rule.typeMode == fW::folderOnly && file.isDir())
                filesToProcess << file;
            else if(rule.typeMode == fW::filesOnly && file.isFile())
                filesToProcess << file;
            else if(rule.typeMode == fW::allEntries)
                filesToProcess << file;
        }
        else if(rule.fieldCondition == rD::nonConditionalMode)
        {
            filesToProcess << file;
        }
    }
    return filesToProcess;
}

QFileInfoList FileWorker::generateFilesList(QString rPath, QStringList paths, bool recursive)
{
    QStringList rPaths;
    if(rPath != QString() && rPath != "Alle")
        rPaths = QStringList(rPath);
    else
        rPaths = paths;

    QFileInfoList allFiles;
    if(!recursive)
    /*Add items to a "QFileInfoList" in a non-recursive manner,
     * which means that directories, excluding their content, will be added
     */
    {
        for(QString path : rPaths)
        {
            QDir dirContent(path);
            allFiles += dirContent.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::System);
        }
    }
    // Add files to a QFileInfoList in a recursive manner, but excludes directories
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

void FileWorker::calcSize(QString path)
{
    if(isBusy)
    {
        emit itemText("Patience my young padawan.");
        return;
    }

    emit sendFolderSizeEntity(folderContentSize(path));
}

void FileWorker::calcSize(QStringList l)
{
    if(isBusy)
    {
        emit itemText("Patience my young padawan.");
        return;
    }
    QList<DirectoryEntity> resultingList = foldersContentSize(l);
    emit sendFolderSizeEntities(resultingList);
}

void FileWorker::countNumberOfFolderItems(QString path, QDir::Filters f, QDirIterator::IteratorFlags i)
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

void FileWorker::countFolders(QStringList Path)
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
        beginProcess();
}

void FileWorker::processFileInformation(QString path)
{
    if(isBusy)
    {
        emit itemText("Fileworker har travlt");
        return;
    }

    QString denotation;
    DirectoryItem item;
    item.path = path;
    double directorySize = convertSizeToAppropriateUnits(folderSize(path),denotation);
    item.dirSize = QString::number(directorySize) + " " + denotation;
    item.numberOfDirectories = folderCount(path);
    item.numberOfFiles = fileCount(path);
    item.directoryContent = scanDir(path);
    item.sufList = getListOfSuffixOccuriencies(path);

    emit processFinished(item);
}

void FileWorker::processFileInformations(QStringList paths)
{
    if(isBusy)
    {
        emit itemText("Fileworker har travlt");
        return;
    }
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
        item.directoryContent = scanDir(p);
        item.sufList = getListOfSuffixOccuriencies(p);
        directories << item;
    }
    emit multipleProcessFinished(directories);
}

void FileWorker::reProcessFileInformation(const QString path)
{

    QString denotation;
    DirectoryItem item;
    item.path = path;
    double directorySize = convertSizeToAppropriateUnits(folderSize(path),denotation);
    item.dirSize = QString::number(directorySize) + " " + denotation;
    item.numberOfDirectories = folderCount(path);
    item.numberOfFiles = fileCount(path);
    item.directoryContent = scanDir(path);
    item.sufList = getListOfSuffixOccuriencies(path);

    emit processFinished(item);
}

void FileWorker::reProcessFileInformations(const QStringList paths)
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
        item.directoryContent = scanDir(p);
        item.sufList = getListOfSuffixOccuriencies(p);
        directories << item;
    }
    emit multipleProcessFinished(directories);
}

