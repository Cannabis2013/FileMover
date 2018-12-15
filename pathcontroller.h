#ifndef PATHCONTROLLER_H
#define PATHCONTROLLER_H

#include <qobject.h>
#include <QStringList>

class pathController : public QObject
{
Q_OBJECT
public:
    // Insert..
    void insertPath(const QString path)
    {
        allPaths << path;
        emit updateList(path);
    }
    void insertPaths(QStringList paths)
    {
        allPaths<< paths;
        emit updateLists(paths);
    }
    //Remove..
    void removePathAt(int i) {allPaths.removeAt(i);}
    QString takePathAt(int i) {return allPaths.takeAt(i);}

    // Get..

    QString getPath(int i) const {return allPaths.at(i);}
    QStringList getAllPaths() const{return allPaths;}

signals:
    void updateList(QString p);
    void updateLists(QStringList p);
private:
    QStringList allPaths;

    friend class settingsController;
};

#endif // PATHCONTROLLER_H
