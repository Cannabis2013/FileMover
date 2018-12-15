#include "fileworkeroperator.h"

fileWorkerOperator::fileWorkerOperator(QObject *parent):
    Worker (parent)
{
    isBusy = false;
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

QList<fileObject> fileWorkerOperator::sizeOfFolderContentItems(QStringList l)
{
    QList<fileObject>resultingList;
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
         fileObject fObject;
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
