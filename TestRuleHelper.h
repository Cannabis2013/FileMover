#ifndef TESTRULEHELPER_H
#define TESTRULEHELPER_H

#include <QCoreApplication>
#include "testfilecreator.h"

class TestRuleHelper
{
public:
    static bool SubRuleEquals(const SubRule &compOne, const SubRule &compTwo)
    {
        if(compOne.copymode != compTwo.copymode ||
                compOne.compareCriteria != compTwo.compareCriteria ||
                compOne.criteria != compTwo.criteria ||
                compOne.matchWholeWords != compTwo.matchWholeWords ||
                compOne.keyWords != compTwo.keyWords)
        {
            return false;
        }

        if(compOne.sizeLimit.first != compTwo.sizeLimit.first ||
                compOne.sizeLimit.second != compTwo.sizeLimit.second)
        {
            return false;
        }

        QPair<int,QString> sIntFirst1 = compOne.sizeInterval.first;
        QPair<int,QString> sIntSecond1 = compOne.sizeInterval.second;

        QPair<int,QString> sIntFirst2 = compTwo.sizeInterval.first;
        QPair<int,QString> sIntSecond2 = compTwo.sizeInterval.second;

        if(sIntFirst1.first != sIntFirst2.first ||
                sIntFirst1.second != sIntFirst2.second)
        {
            return false;
        }

        if(sIntSecond1.first != sIntSecond2.first ||
                sIntSecond1.second != sIntSecond2.second)
        {
            return false;
        }

        if(compOne.date != compTwo.date)
        {
            return false;
        }

        if(compOne.dateIntervals.first != compTwo.dateIntervals.first ||
                compOne.dateIntervals.second != compTwo.dateIntervals.second)
        {
            return false;
        }

        return true;
    }

    static bool SubRuleNotEqual(const SubRule &compOne, const SubRule &compTwo)
    {
        return !SubRuleEquals(compOne,compTwo);
    }

    static bool RuleEquals(const Rule &compOne, const Rule &compTwo)
    {
        if(compOne.title != compTwo.title ||
                compOne.typeFilter != compTwo.typeFilter ||
                compOne.actionRuleEntity != compTwo.actionRuleEntity ||
                compOne.destinationPaths != compTwo.destinationPaths ||
                compOne.appliesToPath != compTwo.appliesToPath ||
                compOne.deepScanMode != compTwo.deepScanMode)
        {
            return false;
        }

        for (int i = 0; i < compOne.subRules.count(); ++i) {
            SubRule sR1 = compOne.subRules.at(i), sR2 = compTwo.subRules.at(i);

            if(!SubRuleEquals(sR1,sR2))
                return false;
        }
        return true;
    }
};

#endif // TESTRULEHELPER_H
