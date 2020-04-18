#include "filesystemwatcher.h"


FileSystemWatcher::FileSystemWatcher(IEntityModelBuilder<DefaultModelInterface, DefaultFileModelList> *entityModelService):
    fileWatcher(new QFileSystemWatcher())
{
    _entityBuilderService = entityModelService;
    connect(fileWatcher,&QFileSystemWatcher::directoryChanged,this,&FileSystemWatcher::changed);
}

void FileSystemWatcher::removePath(const QString &path)
{
    QStringList paths = fileWatcher->directories();
    paths.removeOne(path);
}

void FileSystemWatcher::changed(const QString &path)
{
    auto model = _entityBuilderService->buildFileInformationModel(QStringList() << path);
    emit folderChanged(model);

    QString msg = QString("A change in the following folder has occured: \n %1").arg(path);
    emit sendSystemTrayMessage("Information",msg);
}
