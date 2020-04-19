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
#include "abstractfileinformationmanager.h"

using namespace std;

class FileInformationManager :public AbstractFileInformationManager,
        private AbstractPersistence
{
public:
    FileInformationManager(QString appName, QString orgName);
    ~FileInformationManager();

    void setItemList(const QList<DirectoryItem> &list) override {items = list;}

    bool directoryExists(const QString &path) override;
    QList<DirectoryItem> allItems() const override {return items;}
    DirectoryItem item(const QString &path) const override;
    DirectoryItem itemRef(const QString &path) const override;
    QList<QTreeWidgetItem *> allTreeItems() const override;

    void updateFileInfo(const DirectoryItem &item) override;
    void updateAllFileInfo(const QList<DirectoryItem> &list) override;
    void flush() override;

    QString createTextBrowserHtml(const QString &path) const override;

    QTreeWidgetItem *assembleTreeItems(QTreeWidgetItem *item) const override;

    QList<QTreeWidgetItem *> suffixList(const QString &path) override;

    void readSettings() override;
    void writeSettings() override;


public slots:
    void insertItem(const DirectoryItem &item) override;
    void insertItems(const QList<DirectoryItem> &dirItems) override;

    void replaceItem(const DirectoryItem &newItem) override;

    void removeItem(const QString &path) override;

    void requestItem(const QString &path) override;

private:
    bool isDuplicate(const QString &path) override;

    QList<DirectoryItem>items;
};

#endif // FILEINFORMATION_H
