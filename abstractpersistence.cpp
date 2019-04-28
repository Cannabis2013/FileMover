#include "abstractpersistence.h"

AbstractPersistence::AbstractPersistence(QString appName, QString orgName)
{
    QCoreApplication::setApplicationName(appName);
    QCoreApplication::setOrganizationName(orgName);

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

