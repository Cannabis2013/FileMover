#include "fileworker.h"

fileWorker::fileWorker(processController *pRef, QObject *parent) :
    fileWorkerOperator(parent),pControllerReference(pRef)
{
    busyMessage = "Luke Fileworker har travlt. Vent et øjeblik.";
}

fileWorker::~fileWorker()
{
    delete this;
}

void fileWorker::calcSizeOfIndividualFolderItems(QStringList l)
{
    if(isBusy)
    {
        emit itemText("Fileworker har travlt");
        return;
    }
    QList<fileObject> resultingList = sizeOfFolderContentItems(l);
    emit sendFolderContentItems(resultingList);
}

void fileWorker::beginProcess()
{
    isBusy = true;
    bool isDone = true;
    while(!pControllerReference->queueIsEmpty())
    {
        processItems item = pControllerReference->takeItem();
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

void fileWorker::processFileInformation(QString path)
{
    if(isBusy)
    {
        emit itemText("Fileworker har travlt");
        return;
    }
    QString denotation;
    directoryItem item;
    item.path = path;
    double directorySize = convertSizeToAppropriateUnits(folderSize(path),denotation);
    item.dirSize = QString::number(directorySize) + " " + denotation;
    item.dirCount = folderCount(path);
    item.fileCount = fileCount(path);
    item.allFiles = scanDir(path);
    item.sufList = getListOfSuffixOccuriencies(path);

    emit processFinished(item);
}

void fileWorker::processFileInformations(QStringList paths)
{
    if(isBusy)
    {
        emit itemText("Fileworker har travlt");
        return;
    }
    QList<directoryItem>directories;
    for(QString p: paths)
    {
        QString denotation;
        directoryItem item;
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

void fileWorker::handleProcessRequest()
{
    if(isBusy)
        return;
    else
        beginProcess();
}

void fileWorker::countNumberOfFolderItems(QString path,
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

void fileWorker::countFolders(QStringList Path)
{
    long taeller = 0;
    for (QString p : Path)
    {
         taeller += folderCount(p);
    }
    emit antalFiler(taeller);
}
