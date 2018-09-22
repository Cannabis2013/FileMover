#include "fileinformation.h"

fileInformation::fileInformation()
{
}

bool fileInformation::directoryExists(QString path)
{
    for(directoryItem item : items)
        if(item.path == path)
            return true;
    return false;
}


const directoryItem fileInformation::getItemFromPath(const QString p)
{
    for(directoryItem dI : items)
    {
        if(dI.path == p)
            return dI;
    }
    return directoryItem();
}

void fileInformation::updateFileInfo(directoryItem dI)
{
    QString p = dI.path;
    for (int var = 0; var < items.count(); ++var)
    {
        if(p == items.at(var).path)
            items.replace(var,dI);
    }
}

void fileInformation::updateAllFileInfo(QList<directoryItem> list)
{
    items.clear();
    items.append(list);
}

void fileInformation::flushAll()
{
    items.clear();
}

void fileInformation::removeDirectory(QString p)
{
    for (int var = 0; var < items.count(); ++var)
    {
        if(items.at(var).path == p)
            items.removeAt(var);
    }
}
