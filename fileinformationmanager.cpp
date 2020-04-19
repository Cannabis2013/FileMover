#include "fileinformationmanager.h"

FileInformationManager::FileInformationManager(QString appName, QString orgName):
    AbstractPersistence (appName,orgName)
{
    readSettings();
}

FileInformationManager::~FileInformationManager()
{
    writeSettings();
}

bool FileInformationManager::directoryExists(const QString &path)
{
    for(DirectoryItem item : items)
        if(item.path == path)
            return true;
    return false;
}


DirectoryItem FileInformationManager::item(const QString &path) const
{
    for(DirectoryItem dI : items)
    {
        if(dI.path == path)
            return dI;
    }
    throw QString("Item not found");
}

DirectoryItem FileInformationManager::itemRef(const QString &path) const
{
    for (int i = 0;i < items.count();i++) {
        DirectoryItem item = items.at(i);
        if(item.path == path)
            return items.at(i);
    }
    return DirectoryItem();
}

QList<QTreeWidgetItem*> FileInformationManager::allTreeItems() const
{
    QList<QTreeWidgetItem*> resultingList;
    for (DirectoryItem item : items)
        resultingList.append(assembleTreeItems(item.directoryItemModels));

    return resultingList;
}

void FileInformationManager::updateFileInfo(const DirectoryItem &item)
{
    QString p = item.path;
    for (int var = 0; var < items.count(); ++var)
    {
        if(p == items.at(var).path)
            items.replace(var,item);
    }
}

void FileInformationManager::updateAllFileInfo(const QList<DirectoryItem> &list)
{
    items.clear();
    items.append(list);
}

void FileInformationManager::flush()
{
    items.clear();
}

QString FileInformationManager::createTextBrowserHtml(const QString &path) const
{
    QFileInfo info = path;
    if(!info.isDir())
        return QString();

    DirectoryItem dirItem;

    try {
        dirItem = item(path);
    } catch (QString s) {
        return "Item not found. No information to show.";
    }

    QString tempSize = dirItem.dirSize;
    if(tempSize == QString())
        tempSize = "Not counted";
    QString fC = QString::number(dirItem.numberOfFiles),
            dC = QString::number(dirItem.numberOfDirectories);
#if(_MSC_VER)
    QString resultingHtml = QString(
                        "<body style='background-color:rgb(81,81,81);'>"
                            "<table>"
                                "<td style='padding:5px;background-color:transparent;text-align:center;font-size:24px;font-weight:bold;'>"
                                    "<img src='qrc:/My Images/Ressources/File.png' width=102 height=128 >"
                                    "<p style='color:white;'>%1</p>"
                                "</td>"
                                "<td style='padding:5px;background-color:transparent;text-align:center;font-size:24px;font-weight:bold;'>"
                                    "<img src='qrc:/My Images/Ressources/Folder.png' width=102 height=128>"
                                    "<p style='color:white;'>%2</p>"
                                "</td>"
                                "<td style='padding:5px;background-color:transparent;text-align:center;font-size:24px;font-weight:bold;'>"
                                    "<img src='qrc:/My Images/Ressources/Hdd-icon.png' width=102 height=128>"
                                    "<p style='color:white;'>%3</p>"
                                "</td>"
                            "</table>"
                        "</body>").arg(fC,dC,tempSize);
#elif(__MINGW32__ || __MINGW64__ || __GNUC__)
    QString pth = dirItem.path != "Not defined" ? dirItem.path : "Ikke defineret",
            textColor = "color:white;",
            resultingHtml = QString(
                "<body style='background-image:url(qrc:/My Images/Ressources/solid background black.jpg);'>"
                    "<table>"
                        "<tr>"
                            "<td style='padding:5px;background-color:transparent;text-align:center;font-size:24px;font-weight:bold;'>"
                                "<div style='background-color:gray;'>"
                                    "<img src='qrc:/My Images/Ressources/File.png' width=102 height=128 >"
                                    "<p style='%5'>%1</p>"
                                "</div>"
                            "</td>"
                            "<td style='padding:5px;background-color:transparent;text-align:center;font-size:24px;font-weight:bold;'>"
                                "<div style='background-color:gray;'>"
                                    "<img src='qrc:/My Images/Ressources/Folder.png' width=102 height=128>"
                                    "<p style='%5'>%2</p>"
                                "</div>"
                            "</td>"
                            "<td style='padding:5px;background-color:transparent;text-align:center;font-size:24px;font-weight:bold;'>"
                                "<div style='background-color:gray;'>"
                                    "<img src='qrc:/My Images/Ressources/Hdd-icon.png' width=102 height=128>"
                                    "<p style='%5'>%3</p>"
                                "</div>"
                            "</td>"
                        "</tr>"
                    "</table>"
                    "<div style='float:left;border:2px solid white;%5 font-size: 24px;text-align:left;'"
                        "<p>"
                            "Sti: %4"
                        "</p>"
                    "</div>"
                "</body>").arg(fC,dC,tempSize,pth,textColor);
#endif
    return resultingHtml;

}

QList<QTreeWidgetItem *> FileInformationManager::suffixList(const QString &path)
{
    QList<QTreeWidgetItem*> resultingList;
    for (auto item : items) {
        if(item.path == path)
        {
            for(QPair<QString,int> sufPair : item.sufList)
                resultingList << new QTreeWidgetItem(QStringList {sufPair.first,QString::number(sufPair.second)});

            return resultingList;
        }
    }

    return QList<QTreeWidgetItem*>();
}

void FileInformationManager::insertItem(const DirectoryItem &item)
{
    if(isDuplicate(item.path))
        replaceItem(item);
    else
        items << item;

    emit stateChanged();
}

void FileInformationManager::insertItems(const QList<DirectoryItem> &dirItems)
{
    for (DirectoryItem dirItem : dirItems)
    {
        if(isDuplicate(dirItem.path))
            replaceItem(dirItem);
        else
            items << dirItem;
    }
    emit stateChanged();
}

void FileInformationManager::replaceItem(const DirectoryItem &newItem)
{
    for (int i = 0;i < items.count();i++)
    {
        DirectoryItem item = items.at(i);
        if(item.path == newItem.path)
        {
            items.replace(i,newItem);
            return;
        }
    }
}

void FileInformationManager::removeItem(const QString &path)
{
    for(int i = 0;i < items.count();i++)
    {
        DirectoryItem dirItem = items.at(i);
        if(dirItem.path == path)
        {
            items.removeAt(i);
            emit stateChanged();
            return;
        }
    }
}

void FileInformationManager::requestItem(const QString &path)
{
    for (DirectoryItem item : items)
    {
        if(item.path == path)
        {
            emit sendItem(item);
            return;
        }
    }
}

bool FileInformationManager::isDuplicate(const QString &path)
{
    for(DirectoryItem item : items)
    {
        if(item.path == path)
            return true;
    }

    return false;
}

QTreeWidgetItem *FileInformationManager::assembleTreeItems(QTreeWidgetItem *item) const
{
    QTreeWidgetItem *result = new QTreeWidgetItem;
    for (int i = 0; i < item->columnCount(); ++i)
        result->setText(i,item->text(i));
    if(item->childCount() == 0)
        return result;
    else
    {
        for (int i = 0; i < item->childCount(); ++i)
        {
            if(item->child(i)->childCount() == 0)
            {
                QTreeWidgetItem *child = new QTreeWidgetItem;
                for (int j = 0; j < item->child(i)->columnCount(); ++j)
                    child->setText(j,item->child(i)->text(j));
                result->addChild(child);
            }
            else
            {
                QTreeWidgetItem *child = assembleTreeItems(item->child(i));
                result->addChild(child);
            }
        }
    }
    return result;
}

void FileInformationManager::readSettings()
{

}

void FileInformationManager::writeSettings()
{

}
