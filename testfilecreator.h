#ifndef TESTFILECREATOR_H
#define TESTFILECREATOR_H

#include <qfile.h>
#include <qlist.h>
#include <qfileinfo.h>

extern Q_CORE_EXPORT int qt_ntfs_permission_lookup;

class TestFileCreator
{
public:
    TestFileCreator(const QString &filePath,const QStringList &fileNames, const bool &persist);

    QStringList fileNames() const;

private:
    QStringList _fileNames;
    bool isPersisted = false;


};

#endif // TESTFILECREATOR_H
