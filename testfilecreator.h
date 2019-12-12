#ifndef TESTFILECREATOR_H
#define TESTFILECREATOR_H

#include <qfile.h>
#include <qlist.h>
#include <qfileinfo.h>
#include <qdir.h>
#include <qdiriterator.h>

extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;

const QString dummyContent = "This is a testfile containing only dummy content\n";

class TestFileCreator
{
public:
    TestFileCreator(const QString &filePath,const QStringList &fileNames, const bool &persist);

    QStringList fileNames() const;

    bool emptyTestFolder(QString dirPath = QString());

private:

    void pertainFileList(const QString &directory);

    QStringList _initial_fileNames;
    const QString currentWorkingPath;
    const bool isPersisted;

};

#endif // TESTFILECREATOR_H
