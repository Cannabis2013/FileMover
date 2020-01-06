#ifndef FILEINFORMATION_H
#define FILEINFORMATION_H

#include <QStringList>
#include <QFileInfo>
#include <qlist.h>
#include <qpair.h>
#include <memory>
#include <QTreeWidgetItem>
#include "imutableobject.h"

#include "abstractpersistence.h"

/*
 * Contains meta information related to directories and their content.
 * It provides data for the "detailedFolderView" and "fileInformationView" classes.
 */

using namespace std;

/*
 * The model to be presented in view classes.
 */

struct DirectoryItem
{
    QFileInfoList directoryContent;
    QString dirSize = nullptr;
    int numberOfDirectories;
    long numberOfFiles;
    QString path = "Not defined";
    QList<QPair<QString,int> > sufList;
    QList<QTreeWidgetItem*> suffixItems() const;
    QTreeWidgetItem *directoryItemModels;
};

class FileInformationManager :
        public QObject,
        public IMutableObject,
        private AbstractPersistence
{
    Q_OBJECT
public:
    FileInformationManager(QString appName, QString orgName);
    ~FileInformationManager();

    void setItemList(QList<DirectoryItem> nList){items = nList;}

    bool directoryExists(QString path);
    QList<DirectoryItem> allItems() {return items;}
    DirectoryItem item(QString p);
    DirectoryItem itemRef(QString p);
    QList<QTreeWidgetItem *> allTreeItems();

    void updateFileInfo(DirectoryItem dI);
    void updateAllFileInfo(QList<DirectoryItem> list);
    void flush();

    QString createTextBrowserHtml(QString path);

    void readSettings();
    void writeSettings();

    static QTreeWidgetItem *assembleTreeItems(QTreeWidgetItem *item);

public slots:
    void insertItem(DirectoryItem item);
    void insertItems(QList<DirectoryItem> dirItems);

    void replaceItem(DirectoryItem newItem);

    void removeItem(QString path);

    void requestItem(QString path);

signals:
    void sendItem(DirectoryItem item);
    void stateChanged();
private:

    bool isDuplicate(QString path);

    QList<DirectoryItem>items;
};

#endif // FILEINFORMATION_H
