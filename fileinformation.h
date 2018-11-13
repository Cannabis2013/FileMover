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

struct directoryItem
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
    QString createTextBrowserHtml() const;
};

class fileInformation
{
public:
    fileInformation();

    void insertItem(directoryItem item){items << item;}
    void setItemList(QList<directoryItem> nList){items = nList;}

    bool directoryExists(QString path);
    QList<directoryItem> getAllItems() {return items;}
    directoryItem const getItemFromPath(const QString p);

    void updateFileInfo(directoryItem dI);
    void updateAllFileInfo(QList<directoryItem> list);
    void flushAll();
    void removeDirectory(QString p);
private:
    QList<directoryItem>items;

};

#endif // FILEINFORMATION_H
