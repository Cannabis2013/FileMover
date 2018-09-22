#ifndef FILEINFORMATION_H
#define FILEINFORMATION_H

#include <qstringList.h>
#include <qfileinfo.h>
#include <qlist.h>
#include <iostream>
#include <qpair.h>
#include <memory>

#include <QTreeWidgetItem>

using namespace std;

struct directoryItem
{
    QString dirSize = 0;
    int dirCount;
    long fileCount;
    QString path = "Not defined";
    QList<QPair<QString,int> > sufList;
    QList<QTreeWidgetItem*> suffixItems() const
    {
        QList<QTreeWidgetItem*> resultingList;
        for(QPair<QString,int> sufPair : sufList)
            resultingList << new QTreeWidgetItem(QStringList {sufPair.first,QString::number(sufPair.second)});

        return resultingList;
    }

    QTreeWidgetItem *g(QTreeWidgetItem*item)
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
                    QTreeWidgetItem *child = g(item->child(i));
                    result->addChild(child);
                }
            }
        }
        return result;
    }

    QTreeWidgetItem *allFiles;
    QTreeWidgetItem *treeWidgetItems()
    {
        return g(allFiles);
    }
    QString createTextBrowserHtml() const
    {
        QString tempSize = dirSize;
        if(tempSize == QString())
            tempSize = "Not counted";
        QString fC = QString::number(fileCount),
                dC = QString::number(dirCount);
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
        QString pth = path != "Not defined" ? path : "Ikke defineret",
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
