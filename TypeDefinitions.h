#ifndef TYPEDEFINITIONS_H
#define TYPEDEFINITIONS_H

namespace RRT // Rule Related Types namespace
{
    enum RuleType {action,
                   criteria,
                   compareCriteria,
                   all};

    enum RuleAction{Move,
                    Delete,
                    Copy,
                    none};

    enum RuleCriteria{notDefined,
                             fileBaseMode,
                             filepathMode,
                             fileExtensionMode,
                             fileSize,
                             fileParentMode,
                             fileCreatedMode, // FileDateMode
                             fileModifiedMode, // FileDateMode
                             nonConditionalMode};


    enum RuleCompareCriteria{match,           // FilePathMode/FileNameMode/FileBaseMode/FileExtensionMode
                     dontMatch,             // FilePathMode/FileNameMode/FileBaseMode/FileExtensionMode
                     contains,              // FilePathMode/FileNameMode/FileBaseMode/FileExtensionMode
                     dontContain,           // FilePathMode/FileNameMode/FileBaseMode/FileExtensionMode
                     lesserThan,            // Filesize
                     lesserOrEqualThan,     // Filesize
                     equal,                 // Filesize
                     greaterOrEqualThan,    // Filesize
                     greaterThan,           // Filesize
                     interval,              // Filesize/date interval
                     olderThan,             // FileDateMode {fileCreatedMode,fileModifiedMode}
                     exactDate,             // FileDateMode {fileCreatedMode,fileModifiedMode}
                     youngerThan,           // FileDateMode {fileCreatedMode,fileModifiedMode}
                     noDateSet,             // FileDateMode {fileCreatedMode,fileModifiedMode}
                     noCompareModeSet};

    enum FileTypeEntity {Folder, File, unresolved};
    enum CopyMode{move,copy,noMode};
}


#endif // TYPEDEFINITIONS_H
