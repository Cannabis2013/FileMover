#ifndef DIRECTORYITEM_H
#define DIRECTORYITEM_H

#include <QFileInfoList>
#include <QString>
#include <QTreeWidgetItem>

struct DirectoryItem
{
    QString path = "Not defined";
    QFileInfoList directoryContent;
    QString dirSize = nullptr;
    int numberOfDirectories;
    long numberOfFiles;
    QList<QPair<QString,int> > sufList;
    QTreeWidgetItem *directoryItemModels;
};

#endif // DIRECTORYITEM_H
