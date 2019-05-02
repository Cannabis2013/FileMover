#include "fileworkeroperator.h"

fileWorkerOperator::fileWorkerOperator(QObject *parent):
    Worker (parent)
{
    isBusy = false;
    qt_ntfs_permission_lookup++;
}

bool fileWorkerOperator::removeFileItems(const QFileInfoList filePaths)
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

bool fileWorkerOperator::moveEntities(const QFileInfoList files, const QStringList destinations)
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

bool fileWorkerOperator::copyEntities(const QFileInfoList files, const QStringList destinations)
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

QList<FileObject> fileWorkerOperator::sizeOfFolderContentItems(QStringList l)
{
    QList<FileObject>resultingList;
    long long sZ = 0;
    isBusy = true;

    foreach (QFileInfo path,l)
    {
        QString p = path.absoluteFilePath();
        QDirIterator iT(p,
                        QDir::NoDotAndDotDot |
                        QDir::Files | QDir::System |
                        QDir::Hidden,
                        QDirIterator::Subdirectories);
         while(iT.hasNext())
         {
            QFile f(iT.next());
            sZ += f.size();

            emit itemText(f.fileName());
         }
         FileObject fObject;
         fObject.path = path.filePath();
         fObject.sz = sZ;
         resultingList << fObject;
         sZ = 0;
    }
    isBusy = false;
    return resultingList;
}

void fileWorkerOperator::removeDir(QString &dirName, QStringList &errs)
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

bool fileWorkerOperator::moveRecursively(QString path, QString destination)
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

bool fileWorkerOperator::copyRecursively(QString path, QString destination)
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

int fileWorkerOperator::folderCount(QString p)
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

int fileWorkerOperator::fileCount(QString p)
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

QList<QPair<QString, int> > fileWorkerOperator::getListOfSuffixOccuriencies(QString p)
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

long long fileWorkerOperator::folderSize(QString pf)
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

QTreeWidgetItem *fileWorkerOperator::scanDir(QString p)
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

QStringList fileWorkerOperator::createHeader(QFileInfo fi)
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
