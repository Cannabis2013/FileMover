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
    auto filesChanged = QStringList() << path;
    FileInformationEntity *fEntity;
    auto delegate = eMD::makeFileInformationEntity(filesChanged);
    emit folderChanged(delegate);

    QString msg = QString("A change in the following folder has occured: \n %1").arg(path);
    emit sendSystemTrayMessage("Information",msg);
}
