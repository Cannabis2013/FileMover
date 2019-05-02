#ifndef ABSTRACTPERSISTENCE_H
#define ABSTRACTPERSISTENCE_H

#include <QCoreApplication>
#include <qsettings.h>

class AbstractPersistence
{

public:
    AbstractPersistence(QString appName, QString orgName);
    virtual ~AbstractPersistence() = 0;

    QString OrganisationTitle() const;
    QString ApplicationTitle() const;

    virtual void readSettings() = 0;
    virtual void writeSettings() = 0;

    QSettings *persistenceSettings;

private:
    QString applicationTitle, organisationTitle;
};

#endif // ABSTRACTPERSISTENCE_H
