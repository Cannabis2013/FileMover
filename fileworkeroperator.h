#ifndef FILEWORKEROPERATOR_H
#define FILEWORKEROPERATOR_H

#include "processcontroller.h"

extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;

class fileWorkerOperator : public Worker
{
    Q_OBJECT

public:
    fileWorkerOperator(QObject * parent = nullptr);

    // Public methods
    QStringList static createHeader(QFileInfo fi = QFileInfo());

protected slots:

    // Fileoperation from QFileinfoList..
    bool removeFileItems(const QFileInfoList filePaths);
    bool moveEntities(const QFileInfoList files, const QStringList destinations);
    bool copyEntities(const QFileInfoList files, const QStringList destinations);

signals:
    void itemText(QString iT);
    void antalFiler(long antal);
    void sendFolderContentItems(QList<fileObject> s);

protected:
    /*
     * Protected methods
     * Methods related to calc size of folders and the numbers of them
     */

    QList<fileObject> sizeOfFolderContentItems(QStringList l);

    // Methods to count files and number of contents in folders
    int folderCount(QString p);
    int fileCount(QString p);

    // Methods of calculating size of files and content of folders

    long long folderSize(QString pf);

    // Create a list of suffixes and their occurencies..
    QList<QPair<QString,int> > getListOfSuffixOccuriencies(QString p);

    // Create a list of treeWidgetItem

    QTreeWidgetItem *scanDir(QString p);

    // Protected member variables;
    bool isBusy;

private:

    /*
     * Private recursive methods dealing with fileoperations from filepaths
     */
    void removeDir(QString &dirName, QStringList &errs);
    bool moveRecursively(QString path,
                         QString destination);
    bool copyRecursively(QString path,
                         QString destination);
};

#endif // FILEWORKEROPERATOR_H
