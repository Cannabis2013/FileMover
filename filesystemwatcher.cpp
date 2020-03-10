#include "filesystemwatcher.h"


FileSystemWatcher::FileSystemWatcher():
    fileWatcher(new QFileSystemWatcher())
{
    connect(fileWatcher,&QFileSystemWatcher::directoryChanged,this,&FileSystemWatcher::changed);
}

void FileSystemWatcher::removePath(const QString &path)
{
    QStringList paths = fileWatcher->directories();
    paths.removeOne(path);
}

void FileSystemWatcher::changed(const QString &path)
{
    emit folderChanged(
                DelegateBuilder::buildFileInformationEntity<EntityModel>(
                    QStringList() << path));

    QString msg = QString("A change in the following folder has occured: \n %1").arg(path);
    emit sendSystemTrayMessage("Information",msg);
}
