#include "abstractpersistence.h"

AbstractPersistence::AbstractPersistence(QString appName, QString orgName)
{
    persistenceSettings = new QSettings(orgName,appName);

    applicationTitle = appName;
    organisationTitle = orgName;
}

AbstractPersistence::~AbstractPersistence()
{

}

QString AbstractPersistence::OrganisationTitle() const
{
    return organisationTitle;
}

QString AbstractPersistence::ApplicationTitle() const
{
    return applicationTitle;
}

