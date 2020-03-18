    #ifndef TYPEDEFINITIONS_H
#define TYPEDEFINITIONS_H

#include <QString>

namespace RulesContext // Rule Related Types namespace
{
    enum RuleType {Action = 0x020,
                   Condition = 0x021,
                   CompareCriteria = 0x022,
                   All = 0x023};

    enum RuleAction{MoveAction = 0x024,
                    DeleteAction = 0x025,
                    CopyAction = 0x026,
                    NoAction = 0x01};

    enum RuleCriteria{NotDefined = 0x030,
                             FileBaseMode = 0x031,
                             FileNameMode = 0x032,
                             FileExtensionMode = 0x033,
                             FileSize = 0x034,
                             FileParentMode = 0x035,
                             FileCreatedMode = 0x036, // FileDateMode
                             FileModifiedMode = 0x037, // FileDateMode
                             NonConditionalMode = 0x01};


    enum RuleCompareCriteria{Match = 0x040,           // FilePathMode/FileNameMode/FileBaseMode/FileExtensionMode
                     DontMatch = 0x041,             // FilePathMode/FileNameMode/FileBaseMode/FileExtensionMode
                     Contain = 0x042,              // FilePathMode/FileNameMode/FileBaseMode/FileExtensionMode
                     DontContain = 0x043,           // FilePathMode/FileNameMode/FileBaseMode/FileExtensionMode
                     LesserThan = 0x044,            // Filesize
                     LesserOrEqualThan = 0x045,     // Filesize
                     Equal = 0x046,                 // Filesize
                     GreaterOrEqualThan = 0x047,    // Filesize
                     GreaterThan = 0x048,           // Filesize
                     Interval = 0x049,              // Filesize/date interval
                     OlderThan = 0x050,             // FileDateMode {fileCreatedMode,fileModifiedMode}
                     ExactDate = 0x051,             // FileDateMode {fileCreatedMode,fileModifiedMode}
                     YoungerThan = 0x052,           // FileDateMode {fileCreatedMode,fileModifiedMode}
                     NoCompareModeSet = 0x01};

    enum FileType {Folder = 0x060, File = 0x061,Both = 0x062, Unresolved = 0x01};
    enum CopyMode{Move = 0x065,Copy = 0x066,NoMode = 0x01};
    enum IteratorMode {NonRecursive = 0x080, Recursive = 0x081};
}


#endif // TYPEDEFINITIONS_H
