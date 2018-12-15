#ifndef FILEWORKEROPERATOR_H
#define FILEWORKEROPERATOR_H

#include "processcontroller.h"

class fileWorkerOperator : public Worker
{
    Q_OBJECT

public:
    fileWorkerOperator(QObject * parent = nullptr);

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

    // Protected member variables;
    bool isBusy;

private:

    /*
     * Private recursive methods dealing with fileoperations from filepaths
     */
    void removeDir(QString &dirName, QStringList &errs);
};

#endif // FILEWORKEROPERATOR_H
