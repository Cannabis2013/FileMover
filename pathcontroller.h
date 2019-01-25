#ifndef PATHCONTROLLER_H
#define PATHCONTROLLER_H

#include <qobject.h>
#include <QStringList>

class pathController : public QObject
{
Q_OBJECT
public:
    // Insert..

    void insertPath(QString p)
    {
        paths << p;
    }

    void insertPaths(QStringList p)
    {
        paths << p;
    }
    //Remove..
    void removePathAt(int i) {paths.removeAt(i);}
    QString takePathAt(int i) {return paths.takeAt(i);}

    // Get..

    QString Path(int i) const {return paths.at(i);}
    QStringList Paths() const{return paths;}

private:
    QStringList paths;

    friend class settingsController;
};

#endif // PATHCONTROLLER_H
