#ifndef FILEINFORMATION_H
#define FILEINFORMATION_H

#include <QStringList>
#include <QFileInfo>
#include <qlist.h>
#include <iostream>
#include <qpair.h>
#include <memory>
#include <QTreeWidgetItem>

#include "abstractpersistence.h"

/*
 * It provides data for the "detailedFolderView" and "fileInformationView" classes.
 */

using namespace std;

/*
 * The model to be presented in view classes.
 */

struct DirectoryItem
{
    QString dirSize = nullptr;
    int dirCount;
    long fileCount;
    QString path = "Not defined";
    QList<QPair<QString,int> > sufList;
    QList<QTreeWidgetItem*> suffixItems() const;
    QTreeWidgetItem *allFiles;
};

class FileInformationManager : public QObject, public AbstractPersistence
{
    Q_OBJECT
public:
    FileInformationManager(QString appName, QString orgName);
    ~FileInformationManager();

    void setItemList(QList<DirectoryItem> nList){items = nList;}

    bool directoryExists(QString path);
    QList<DirectoryItem> allItems() {return items;}
    DirectoryItem item(QString p);
    QList<QTreeWidgetItem *> allTreeItems();

    void updateFileInfo(DirectoryItem dI);
    void updateAllFileInfo(QList<DirectoryItem> list);
    void flushAll();

    QString createTextBrowserHtml(QString path);

    void readSettings();
    void writeSettings();

public slots:
    void insertItem(DirectoryItem item);
    void insertItems(QList<DirectoryItem> items);

    void removeItem(QString path);

signals:
    void stateChanged();
private:
    QList<DirectoryItem>items;
};

#endif // FILEINFORMATION_H
