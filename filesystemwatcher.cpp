#include "filesystemwatcher.h"


FileSystemWatcher::FileSystemWatcher(const QStringList &paths):
    fWatcher(new QFileSystemWatcher(paths))
{
    connect(fWatcher,&QFileSystemWatcher::directoryChanged,this,&FileSystemWatcher::changed);
}

void FileSystemWatcher::removePath(const QString &path)
{
    QStringList paths = fWatcher->directories();
    paths.removeOne(path);
}

void FileSystemWatcher::changed(const QString &path)
{
    FileInformationEntity *fEntity;
    try {
        fEntity = makeEntity<FileInformationEntity>(fileInformationEntity);
    } catch (const char *msg) {
        cout << msg << endl;
        exit(1);
    }
    fEntity->filePaths << path;
    emit folderChanged(fEntity);
    QString msg = QString("A change in the following folder has occured: \n %1").arg(path);
    emit sendSystemTrayMessage("Information",msg);
}
