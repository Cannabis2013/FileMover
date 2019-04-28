#ifndef ABSTRACTPERSISTENCE_H
#define ABSTRACTPERSISTENCE_H

#include <QCoreApplication>
#include <qsettings.h>

class AbstractPersistence
{
public:
    AbstractPersistence(QString appName, QString orgName);
    virtual ~AbstractPersistence();

    QString OrganisationTitle() const;
    QString ApplicationTitle() const;

protected:
    virtual void readSettings() = 0;
    virtual void writeSettings() = 0;

private:
    QString applicationTitle, organisationTitle;
};

#endif // ABSTRACTPERSISTENCE_H
