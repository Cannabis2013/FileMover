#ifndef IRULEDEFINITIONS_H
#define IRULEDEFINITIONS_H

#include <qlist.h>

class IRuleDefinitions
{
public:

    virtual const QStringList intervalConditionalList() = 0;

    virtual const QStringList buildStringListFromEntity(const int &property) = 0;
    virtual const QStringList fileCompareModesToStringList(const int &condition) = 0;
    virtual const QStringList allFileTypeEntitiesToStrings() = 0;

    virtual QString fileActionEntityToString(const int &mode) = 0;
    virtual int fileActionEntityFromString(const QString mode) = 0;
    virtual QString buildStringFromCriteria(const int &mode) = 0;
    virtual int buildCriteriaFromString(const QString string) = 0;
    virtual QString buildStringFromCompareCriteria(const int &mode) = 0;
    virtual int fileCompareEntityFromString(const QString string) = 0;
    virtual QString fileTypeCriteriaToString(const int &type) = 0;
    virtual int fileTypeCriteriaFromString(const QString &string) = 0;

    virtual QStringList sizeUnits() = 0;
    virtual QString buildDefaultStringValue() = 0;
};
#endif // IRULEDEFINITIONS_H
