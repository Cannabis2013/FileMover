#ifndef ABSTRACTPERSISTENCE_H
#define ABSTRACTPERSISTENCE_H

#include <QCoreApplication>
#include <qsettings.h>

class AbstractPersistence
{

public:
    AbstractPersistence(QString appName, QString orgName)
    {
        _persistenceSettings = new QSettings(orgName,appName);

        applicationTitle = appName;
        organisationTitle = orgName;
    }
    virtual ~AbstractPersistence() = default;

    QString OrganisationTitle() const
    {
        return organisationTitle;
    }
    QString ApplicationTitle() const
    {
        return applicationTitle;
    }

    virtual void readSettings() = 0;
    virtual void writeSettings() = 0;


    QSettings *persistenceSettings() const
    {
        return _persistenceSettings;
    }

private:
    QSettings *_persistenceSettings;
    QString applicationTitle, organisationTitle;
};

#endif // ABSTRACTPERSISTENCE_H
