#include "fileworker.h"

fileWorker::fileWorker(processController *pRef, QObject *parent) :
    fileWorkerOperator(parent),pControllerReference(pRef)
{
    busyMessage = "Luke Fileworker har travlt. Vent et øjeblik.";
    qt_ntfs_permission_lookup++;
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
    while(!pControllerReference->queueIsEmpty())
    {
        processItems item = pControllerReference->takeItem();
        if(item.ruleMode == rD::Delete || item.ruleMode == rD::none)
            removeFileItems(item.list);
        else if(item.ruleMode == rD::Move)
            moveEntities(item.list,item.destinations);
        else if(item.ruleMode == rD::Copy)
            copyEntities(item.list,item.destinations);
    }
    isBusy = false;
    emit jobDone(true);
}


QTreeWidgetItem *fileWorker::scanDir(QString p)
{
QFileInfo info(p);
if(!info.isDir() && !info.isFile())
    return nullptr;
QTreeWidgetItem *result = new QTreeWidgetItem(createHeader(p));
QDirIterator ite(p);
while(ite.hasNext())
{
    QFileInfo fInfo = ite.next();
    if(fInfo.fileName() == "." || fInfo.fileName() == "..")
    {}
    else if(fInfo.isDir())
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
    item.sufList = countFileSuffixes(path);

    emit processFinished(item);
}

void fileWorker::processDirectoryInformation(directoryItem item)
{
    if(isBusy)
    {
        emit infoReport(busyMessage);
        return;
    }
    emit infoReport("Programmet er gået i gang med at udregne diverse størrelser. /n Hav tålmodighed. :-D");
    QString p = item.path,
            denotation;
    double directorySize = convertSizeToAppropriateUnits(folderSize(p),denotation);
    item.dirSize = QString::number(directorySize) + " " + denotation;

    item.dirCount = folderCount(p);
    item.fileCount = fileCount(p);
    item.allFiles = scanDir(p);
    item.sufList = countFileSuffixes(p);

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
        item.sufList = countFileSuffixes(p);
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

long long fileWorker::folderSize(QString pf)
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

int fileWorker::folderCount(QString p)
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

int fileWorker::fileCount(QString p)
{
    QFileInfo info = p;
    QDirIterator ite(info.absoluteFilePath(),
                     QDir::NoDotAndDotDot |
                     QDir::NoDot | QDir::Files,
                     QDirIterator::Subdirectories);
    int taeller = 0;
    while(ite.hasNext())
    {
        QFileInfo i = ite.fileInfo();
        taeller = i.isFile() ? ++taeller : taeller;
        emit itemText(QString("%1 files").arg(QString::number(taeller)));
        ite.next();
    }
    return taeller;
}

QList<QPair<QString, int> > fileWorker::countFileSuffixes(QString p)
{
QList<QPair<QString, int> > resultingList;
QStringList allSufs,tempSufs;
QDirIterator ite(p,
                 QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Files,
                 QDirIterator::Subdirectories);
while(ite.hasNext())
{
    QFileInfo i = ite.fileInfo();
    if(i.isFile())
    {
        if(i.suffix() == "")
            allSufs << "Unknown";
        else
            allSufs << i.suffix();
    }
    ite.next();
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



QStringList fileWorker::createHeader(QFileInfo fi)
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
        headers << fi.fileName();
        headers << fi.absolutePath();
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

void fileWorker::countFolders(QStringList Path,
                          QDir::Filters f,
                          QDirIterator::IteratorFlags i)
{
    long taeller = 0;
    for (QString p : Path)
    {
        QDirIterator iT(p,f,i);
        while(iT.hasNext())
        {
            QFileInfo fil = iT.next();
            if(!fil.isDir())
                taeller++;
        }
    }
    emit antalFiler(taeller);
}

void processController::addToQueue(processItems pI)
{
    insertItem(pI);
}

processController::~processController()
{
    delete this;
}
