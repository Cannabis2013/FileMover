#ifndef ABSTRACTFILEINFORMATIONMANAGER_H
#define ABSTRACTFILEINFORMATIONMANAGER_H

#include <QTreeWidgetItem>
#include "imutableobject.h"
#include "directoryitem.h"


class AbstractFileInformationManager : public QObject,
                                       public IMutableObject
{
    Q_OBJECT
public:

    virtual void setItemList(const QList<DirectoryItem> &list) = 0;

    virtual bool directoryExists(const QString &path) = 0;
    virtual QList<DirectoryItem> allItems() const = 0;
    virtual DirectoryItem item(const QString &path) const = 0;
    virtual DirectoryItem itemRef(const QString &path) const = 0;
    virtual QList<QTreeWidgetItem *> allTreeItems() const = 0;

    virtual void updateFileInfo(const DirectoryItem &item) = 0;
    virtual void updateAllFileInfo(const QList<DirectoryItem> &list) = 0;
    virtual void flush() = 0;

    virtual QString createTextBrowserHtml(const QString &path) const = 0;

    virtual QTreeWidgetItem *assembleTreeItems(QTreeWidgetItem *item) const = 0;

    virtual QList<QTreeWidgetItem*> suffixList(const QString &path) = 0;

public slots:
    virtual void insertItem(const DirectoryItem &item) = 0;
    virtual void insertItems(const QList<DirectoryItem> &dirItems) = 0;

    virtual void replaceItem(const DirectoryItem &item) = 0;

    virtual void removeItem(const QString &path) = 0;

    virtual void requestItem(const QString &path) = 0;

signals:
    void sendItem(const DirectoryItem &item);
    void stateChanged();

private:
    virtual bool isDuplicate(const QString &path) = 0;
};

#endif // ABSTRACTFILEINFORMATIONMANAGER_H
