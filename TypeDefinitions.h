    #ifndef TYPEDEFINITIONS_H
#define TYPEDEFINITIONS_H


namespace RulesContext // Rule Related Types namespace
{
    enum RuleType {Action = 0x020,
                   Criteria = 0x021,
                   CompareCriteria = 0x022,
                   All = 0x023};

    enum RuleAction{MoveAction = 0x024,
                    DeleteAction = 0x025,
                    CopyAction = 0x026,
                    NoAction = 0x01};

    enum RuleCriteria{notDefined = 0x030,
                             fileBaseMode = 0x031,
                             filepathMode = 0x032,
                             fileExtensionMode = 0x033,
                             fileSize = 0x034,
                             fileParentMode = 0x035,
                             fileCreatedMode = 0x036, // FileDateMode
                             fileModifiedMode = 0x037, // FileDateMode
                             nonConditionalMode = 0x01};


    enum RuleCompareCriteria{match = 0x040,           // FilePathMode/FileNameMode/FileBaseMode/FileExtensionMode
                     dontMatch = 0x041,             // FilePathMode/FileNameMode/FileBaseMode/FileExtensionMode
                     contains = 0x042,              // FilePathMode/FileNameMode/FileBaseMode/FileExtensionMode
                     dontContain = 0x043,           // FilePathMode/FileNameMode/FileBaseMode/FileExtensionMode
                     lesserThan = 0x044,            // Filesize
                     lesserOrEqualThan = 0x045,     // Filesize
                     equal = 0x046,                 // Filesize
                     greaterOrEqualThan = 0x047,    // Filesize
                     greaterThan = 0x048,           // Filesize
                     interval = 0x049,              // Filesize/date interval
                     olderThan = 0x050,             // FileDateMode {fileCreatedMode,fileModifiedMode}
                     exactDate = 0x051,             // FileDateMode {fileCreatedMode,fileModifiedMode}
                     youngerThan = 0x052,           // FileDateMode {fileCreatedMode,fileModifiedMode}
                     noCompareModeSet = 0x01};

    enum FileType {Folder = 0x060, File = 0x061,Both = 0x062, Unresolved = 0x01};
    enum CopyMode{Move = 0x065,Copy = 0x066,NoMode = 0x01};
    enum IteratorMode {NonRecursive = 0x080, Recursive = 0x081};
}


#endif // TYPEDEFINITIONS_H
