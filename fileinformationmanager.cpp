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

bool FileInformationManager::directoryExists(QString path)
{
    for(DirectoryItem item : items)
        if(item.path == path)
            return true;
    return false;
}


DirectoryItem FileInformationManager::item(QString p)
{
    for(DirectoryItem dI : items)
    {
        if(dI.path == p)
            return dI;
    }
    throw QString("Item not found");
}

QList<QTreeWidgetItem*> FileInformationManager::allTreeItems()
{
    QList<QTreeWidgetItem*> resultingList;
    for (DirectoryItem item : items)
        resultingList.append(assembleTreeItems(item.directoryContent));

    return resultingList;
}

void FileInformationManager::updateFileInfo(DirectoryItem dI)
{
    QString p = dI.path;
    for (int var = 0; var < items.count(); ++var)
    {
        if(p == items.at(var).path)
            items.replace(var,dI);
    }
}

void FileInformationManager::updateAllFileInfo(QList<DirectoryItem> list)
{
    items.clear();
    items.append(list);
}

void FileInformationManager::flushAll()
{
    items.clear();
}

QString FileInformationManager::createTextBrowserHtml(QString path)
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
    QString fC = QString::number(dirItem.fileCount),
            dC = QString::number(dirItem.dirCount);
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
#elif(__MINGW32__)
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

void FileInformationManager::insertItem(DirectoryItem item)
{
    items << item;
    emit stateChanged();
}

void FileInformationManager::insertItems(QList<DirectoryItem> items)
{
    this->items << items;
    emit stateChanged();
}

void FileInformationManager::removeItem(QString path)
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

QList<QTreeWidgetItem *> DirectoryItem::suffixItems() const
{
    QList<QTreeWidgetItem*> resultingList;
    for(QPair<QString,int> sufPair : sufList)
        resultingList << new QTreeWidgetItem(QStringList {sufPair.first,QString::number(sufPair.second)});

    return resultingList;
}

QTreeWidgetItem *FileInformationManager::assembleTreeItems(QTreeWidgetItem *item)
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
