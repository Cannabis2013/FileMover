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
    wrk::iteratorMode typeMode = fW::noTypeSet;
    rD::fileFieldCondition fieldCondition = rD::nonConditionalMode;

    QPair<int,QString>sizeLimit;
    QPair<QPair<int,QString>,QPair<int,QString>> sizeIntervalLimits;
    QPair<rD::compareMode,myDateTime>fixedDate;
    QPair<myDateTime,myDateTime>intervalDate;
    bool matchWholeWords = false;
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
        bool condition = false;

        QFileInfoList filesToProcess;
        for(QFileInfo file : fileList)
        {

            // Evaluating filename patterns
            if(fieldCondition == rD::filepathMode)
            {
                if(fileCompareMode == rD::contains)
                {
                    for(QString kWord : keyWords)
                        if(file.fileName().contains(kWord))
                            condition = true;
                    if(condition)
                        filesToProcess << file;
                }
                else if(fileCompareMode == rD::dontContain)
                {
                    for(QString kWord : keyWords)
                        if(file.fileName().contains(kWord))
                            condition = true;
                    if(!condition)
                        filesToProcess << file;
                }
                else if(fileCompareMode == rD::match)
                {
                    for(QString kWord : keyWords)
                        if(file.fileName() == kWord)
                            condition = true;
                    if(condition)
                        filesToProcess << file;
                }
                else if(fileCompareMode == rD::dontMatch)
                {
                    for(QString kWord : keyWords)
                        if(file.fileName() == kWord)
                            condition = true;
                    if(!condition)
                        filesToProcess << file;
                }
            }

            // Evaluating file extension related patterns
            else if(fieldCondition == rD::extensionMode && file.isFile())
            {
                if(fileCompareMode == rD::contains)
                {
                    for(QString kWord : keyWords)
                        if(file.suffix().contains(kWord))
                            condition = true;
                    if(condition)
                        filesToProcess << file;
                }
                else if(fileCompareMode == rD::dontContain)
                {
                    for(QString kWord : keyWords)
                        if(file.suffix().contains(kWord))
                            condition = true;
                    if(!condition)
                        filesToProcess << file;
                }
                else if(fileCompareMode == rD::match)
                {
                    for(QString kWord : keyWords)
                        if(file.suffix() == kWord)
                            condition = true;
                    if(condition)
                        filesToProcess << file;
                }
                else if(fileCompareMode == rD::dontMatch)
                {
                    for(QString kWord : keyWords)
                        if(file.suffix() == kWord)
                            condition = true;
                    if(!condition)
                        filesToProcess << file;
                }
            }
            else if(fieldCondition == rD::sizeMode)
            {
                if(fileCompareMode != rD::interval)
                {
                    if(fileCompareMode == rD::lesser && file.size() < fW::byteConvert(sizeLimit.first,sizeLimit.second))
                        filesToProcess << file;
                    else if(fileCompareMode == rD::lesserOrEqual &&
                            file.size() <= fW::byteConvert(sizeLimit.first,sizeLimit.second))
                        filesToProcess << file;
                    else if(fileCompareMode == rD::equal &&
                            file.size() == fW::byteConvert(sizeLimit.first,sizeLimit.second))
                        filesToProcess << file;
                    else if(fileCompareMode == rD::biggerOrEqual &&
                            file.size() >= fW::byteConvert(sizeLimit.first,sizeLimit.second))
                        filesToProcess << file;
                    else if(fileCompareMode == rD::bigger &&
                            file.size() > fW::byteConvert(sizeLimit.first,sizeLimit.second))
                        filesToProcess << file;
                }
                else if(fileCompareMode == rD::interval &&
                        file.size() >= fW::byteConvert(sizeIntervalLimits.first.first,sizeIntervalLimits.first.second) &&
                        file.size() <= fW::byteConvert(sizeIntervalLimits.second.first,sizeIntervalLimits.second.second))
                    filesToProcess << file;

            }
            else if(fieldCondition == rD::dateCreatedMode)
            {
                if(fileCompareMode == rD::interval)
                {
                    if(intervalDate.first > file.created() && intervalDate.second < file.created())
                        filesToProcess << file;
                }
                else if(fileCompareMode != rD::interval)
                {
                    if(fileCompareMode == rD::youngerThan && fixedDate.second > file.created())
                        filesToProcess << file;
                    else if(fileCompareMode == rD::exactDate && fixedDate.second== file.created())
                        filesToProcess << file;
                    else if(fileCompareMode == rD::olderThan && fixedDate.second < file.created())
                        filesToProcess << file;
                }

            }
            else if(fieldCondition == rD::dateModifiedMode)
            {
                if(fileCompareMode == rD::interval)
                {
                    if(intervalDate.first > file.lastModified() && intervalDate.second < file.lastModified())
                        filesToProcess << file;
                }
                else if(fileCompareMode != rD::interval)
                {
                    if(fileCompareMode == rD::youngerThan && fixedDate.second > file.lastModified())
                        filesToProcess << file;
                    else if(fileCompareMode == rD::exactDate && fixedDate.second == file.lastModified())
                        filesToProcess << file;
                    else if(fileCompareMode == rD::olderThan && fixedDate.second < file.lastModified())
                        filesToProcess << file;
                }
            }
            //
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
    QString identification,title = "title";
    rD::fileActionRule actionRule;
    QStringList destinationPath;
    QString appliesToPath = "Alle";
    bool deepScanMode = false;
    QList<subRule> subRules;
    rD::fileActionRule actionFromString();
};


#endif // RULES_H
