#ifndef RULES_H
#define RULES_H

#include <fileworker.h>
#include <QPair>
#include <QStringList>

struct subRule
{
    // Enumerated values..
    rD::copyMode copymode = rD::noMode;
    rD::compareMode fileCompareMode = rD::noCompareModeSet;
    bW::iteratorMode typeMode = fW::noTypeSet;
    rD::fileFieldCondition fieldCondition = rD::nonConditionalMode;

    QPair<int,QString>sizeLimit;
    QPair<QPair<int,QString>,QPair<int,QString>> sizeIntervalLimits;
    QPair<rD::compareMode,myDateTime>fixedDate;
    QPair<myDateTime,myDateTime>intervalDate;
    bool matchWholeWords = false, extension = false;
    QStringList keyWords;

    // Methods..

    QString sizeLimitsToString()
    {
        QString minSize = QString::number(sizeIntervalLimits.first.first),
                maxSize = QString::number(sizeIntervalLimits.second.first);
        QString sizeUnitMin = sizeIntervalLimits.first.second,
                sizeUnitMax = sizeIntervalLimits.second.second;
        return "Min: " + minSize + " " + sizeUnitMin
                + " " + "max: " + maxSize + " " + sizeUnitMax;
    }
    QString dateLimitsToString()
    {
        QString startDate = intervalDate.first.date().toString("dd.MM.yyyy"),
                endDate = intervalDate.second.date().toString("dd.MM.yyyy");
        return "Start dato: " + startDate + " slut dato: " + endDate;
    }
    QString Keyword()
    {
        if(fieldCondition == rD::sizeMode &&
                fileCompareMode != rD::interval)
            return QString::number(sizeLimit.first) + " " + sizeLimit.second;
        else if(fieldCondition == rD::sizeMode &&
                fileCompareMode == rD::interval)
            return sizeLimitsToString();
        else if((fieldCondition == rD::dateCreatedMode || fieldCondition == rD::dateModifiedMode) &&
                fileCompareMode != rD::interval)
            return fixedDate.second.toString("dd.MM.yyyy");
        else if((fieldCondition == rD::dateCreatedMode || fieldCondition == rD::dateModifiedMode) &&
                fileCompareMode == rD::interval)
            return dateLimitsToString();
        else if(fieldCondition == rD::typeMode)
            return rD::typeFromEnum(typeMode);
        else
            return fW::mergeStringList(keyWords);
    }
    QFileInfoList processList(QFileInfoList fileList)
    {
        QFileInfoList filesToProcess;
        for(QFileInfo file : fileList)
        {
            if(fieldCondition == rD::filepathMode &&
                    fileCompareMode == rD::contains)
            {
                bool contains = false;
                for(QString kWord : keyWords)
                {
                    if(file.fileName().contains(kWord))
                        contains = true;
                }
                if(contains)
                    filesToProcess << file;
            }
            else if(fieldCondition == rD::filepathMode &&
                    fileCompareMode == rD::dontContain)
            {
                bool dontContain = false;
                for(QString kWord : keyWords)
                {
                    if(!file.fileName().contains(kWord))
                        dontContain = true;
                }
                if(dontContain)
                    filesToProcess << file;
            }
            else if(fieldCondition == rD::filepathMode &&
                    fileCompareMode == rD::match)
            {
                bool match = false;
                for(QString kWord : keyWords)
                {
                    if(file.fileName() == kWord)
                        match = true;
                }
                if(match)
                    filesToProcess << file;
            }
            else if(fieldCondition == rD::filepathMode &&
                    fileCompareMode == rD::dontMatch)
            {
                bool dontMatch = false;
                for(QString kWord : keyWords)
                {
                    if(file.fileName() != kWord)
                        dontMatch = true;
                }
                if(dontMatch)
                    filesToProcess << file;
            }
            else if(fieldCondition == rD::sizeMode && fileCompareMode != rD::interval)
            {
                if(fileCompareMode == rD::lesser && file.size() < fW::byteConvert(sizeLimit.first,sizeLimit.second))
                {
                    filesToProcess << file;
                }
                else if(fileCompareMode == rD::lesserOrEqual && file.size() <= fW::byteConvert(sizeLimit.first,sizeLimit.second))
                    filesToProcess << file;
                else if(fileCompareMode == rD::equal && file.size() == fW::byteConvert(sizeLimit.first,sizeLimit.second))
                    filesToProcess << file;
                else if(fileCompareMode == rD::biggerOrEqual && file.size() >= fW::byteConvert(sizeLimit.first,sizeLimit.second))
                    filesToProcess << file;
                else if(fileCompareMode == rD::bigger && file.size() > fW::byteConvert(sizeLimit.first,sizeLimit.second))
                    filesToProcess << file;
            }
            else if(fieldCondition == rD::sizeMode &&
                    fileCompareMode == rD::interval &&
                    file.size() >= fW::byteConvert(sizeIntervalLimits.first.first,sizeIntervalLimits.first.second) &&
                    file.size() <= fW::byteConvert(sizeIntervalLimits.second.first,sizeIntervalLimits.second.second))
            {
                filesToProcess << file;
            }
            else if(fieldCondition == rD::dateCreatedMode && fileCompareMode != rD::interval)
            {
                if(fileCompareMode == rD::youngerThan && fixedDate.second > file.created())
                    filesToProcess << file;
                else if(fileCompareMode == rD::exactDate && fixedDate.second== file.created())
                    filesToProcess << file;
                else if(fileCompareMode == rD::olderThan && fixedDate.second < file.created())
                    filesToProcess << file;
            }
            else if(fieldCondition == rD::dateCreatedMode && fileCompareMode == rD::interval)
            {
                if(intervalDate.first > file.created() && intervalDate.second < file.created())
                    filesToProcess << file;
            }
            else if(fieldCondition == rD::dateModifiedMode && fileCompareMode != rD::interval)
            {
                if(fileCompareMode == rD::youngerThan && fixedDate.second > file.lastModified())
                    filesToProcess << file;
                else if(fileCompareMode == rD::exactDate && fixedDate.second == file.lastModified())
                    filesToProcess << file;
                else if(fileCompareMode == rD::olderThan && fixedDate.second < file.lastModified())
                    filesToProcess << file;
            }
            else if(fieldCondition == rD::dateModifiedMode && fileCompareMode == rD::interval)
            {
                if(intervalDate.first > file.lastModified() && intervalDate.second < file.lastModified())
                    filesToProcess << file;
            }
            else if(fieldCondition == rD::typeMode)
            {
                if(typeMode == fW::folderOnly && file.isDir())
                    filesToProcess << file;
                else if(typeMode == fW::filesOnly && file.isFile())
                    filesToProcess << file;
                else if(typeMode == fW::allEntries)
                    filesToProcess << file;
            }
            else if(fieldCondition == rD::nonConditionalMode)
            {
                filesToProcess << file;
            }
        }
        return filesToProcess;
    }
};

struct rule
{
    QString identification,title = "title",action = "none";
    rD::fileActionRule actionRule;
    QStringList destinationPath;
    QString appliesToPath = "Alle";
    bool deepScanMode = false;
    QList<subRule> subRules;
    rD::fileActionRule actionFromString()
    {
        return rD::actionFromString(action);
    }

};


#endif // RULES_H
