#include "fileworker.h"

FileWorker::FileWorker(ProcessManager *pRef, QObject *parent) :
    fileWorkerOperator(parent),pControllerReference(pRef)
{
    busyMessage = "Luke Fileworker har travlt. Vent et øjeblik.";
}

FileWorker::~FileWorker()
{
    delete this;
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

void FileWorker::calcSizeOfIndividualFolderItems(QStringList l)
{
    if(isBusy)
    {
        emit itemText("Fileworker har travlt");
        return;
    }
    QList<FileObject> resultingList = sizeOfFolderContentItems(l);
    emit sendFolderContentItems(resultingList);
}

void FileWorker::beginProcess()
{
    isBusy = true;
    bool isDone = true;
    while(!pControllerReference->queueIsEmpty())
    {
        ProcessItems item = pControllerReference->takeItem();
        if(item.ruleMode == rD::Delete || item.ruleMode == rD::none)
            isDone = removeFileItems(item.list) ? isDone : false;
        else if(item.ruleMode == rD::Move)
            isDone = moveEntities(item.list,item.destinations) ? isDone : false;
        else if(item.ruleMode == rD::Copy)
            isDone = copyEntities(item.list,item.destinations) ? isDone : false;
    }
    isBusy = false;
    emit jobDone(isDone);
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
    item.dirCount = folderCount(path);
    item.fileCount = fileCount(path);
    item.allFiles = scanDir(path);
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
        item.dirCount = folderCount(p);
        item.fileCount = fileCount(p);
        item.allFiles = scanDir(p);
        item.sufList = getListOfSuffixOccuriencies(p);
        directories << item;
    }
    emit multipleProcessFinished(directories);
}

void FileWorker::handleProcessRequest()
{
    if(isBusy)
        return;
    else
        beginProcess();
}

void FileWorker::countNumberOfFolderItems(QString path,
                         QDir::Filters f,
                         QDirIterator::IteratorFlags i)
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
    emit antalFiler(taeller);
    isBusy = false;
}

void FileWorker::countFolders(QStringList Path)
{
    long taeller = 0;
    for (QString p : Path)
    {
         taeller += folderCount(p);
    }
    emit antalFiler(taeller);
}
