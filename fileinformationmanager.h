#ifndef FILEINFORMATION_H
#define FILEINFORMATION_H

#include <QStringList>
#include <QFileInfo>
#include <qlist.h>
#include <iostream>
#include <qpair.h>
#include <memory>
#include <QTreeWidgetItem>

/*
 * This class generates infortion about the current folder in the "informationView" widget.
 *
*/
using namespace std;

struct DirectoryItem
{
    QString dirSize = nullptr;
    int dirCount;
    long fileCount;
    QString path = "Not defined";
    QList<QPair<QString,int> > sufList;
    QList<QTreeWidgetItem*> suffixItems() const;

    QTreeWidgetItem *g(QTreeWidgetItem*item) const;

    QTreeWidgetItem *allFiles;
    QTreeWidgetItem *treeWidgetItems()
    {
        return g(allFiles);
    }
};

class FileInformationManager
{
public:
    FileInformationManager();

    void insertItem(DirectoryItem item){items << item;}
    void insertItems(QList<DirectoryItem> items) {this->items << items;}
    void setItemList(QList<DirectoryItem> nList){items = nList;}

    bool directoryExists(QString path);
    QList<DirectoryItem> allItems() {return items;}
    DirectoryItem item(QString p);

    void updateFileInfo(DirectoryItem dI);
    void updateAllFileInfo(QList<DirectoryItem> list);
    void flushAll();
    void removeDirectory(QString p);

    QString createTextBrowserHtml(QString path);
private:
    QList<DirectoryItem>items;
};

#endif // FILEINFORMATION_H
