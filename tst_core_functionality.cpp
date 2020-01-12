#include <QtTest>
#include <QCoreApplication>


#include "testfilecreator.h"

#ifdef TEST_MODE

#define SLEEP_SEC 1
#define DELETE_STATUS "Some files/folders not deleted"

class RuleTools
{
public:
    static bool SubRuleEquals(const SubRule &compOne, const SubRule &compTwo)
    {
        if(compOne.copymode != compTwo.copymode ||
                compOne.fileCompareMode != compTwo.fileCompareMode ||
                compOne.fieldCondition != compTwo.fieldCondition ||
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

#ifdef __WIN64__
#define TEST_WORKING_PATH "d:/Test_folder/"
#elif __GNUC__
    #define TEST_WORKING_PATH QDir::homePath() + "/MyDocs/Programming/Test/FileMover_Test_Folder/"
#endif

const QStringList test_file_set_1 = {"test.fdl",
                               "Notes.txt",
                               "hello.cpp",
                               "hello.h",
                               "trance2019.pls",
                               "DTU_Mat1_Enotes.pdf",
                               "README.txt",
                               "DJ Crack - Elmers song.mp3",
                               "FCK.jpg",
                               "Blomst.jpg",
                               "Mithjem.png",
                               "MyClass.jar",
                               "FileHandler.jar",
                               "MyPage.cshtml"};

class Core_functionality : public QObject
{
    Q_OBJECT

public:
    Core_functionality();
    ~Core_functionality();

private slots:

    // Cleanup

    void cleanup();

    void cleanupTestCase();

    // Settings and persistence related
    void persistence_SettingsManager_Success_1();
    void persistence_SettingsManager_Success_2();

    /*
     * Rules section
     */

    void insert_rule_filepath_match_success_1();
    void insert_rule_filepath_match_fail_1();
    void insert_rule_filepath_match_fail_2();
    void insert_rule_datecreated_before_succes1();
    void insert_rule_datecreated_after_succes1();
    void insert_rule_datecreated_before_fail1();
    void insert_rule_datecreated_before_fail2();
    void insert_rule_sizeinterval_success_1();
    void insert_rule_sizeinterval_fail_1();

    /*
     * FileOperations section
     */

    /*
     * Delete
     */

    // FilePathMode
    void operation_filepath_match_success_1();
    void operation_filepath_match_fail_1();
    void operation_filepath_contain_success_1();
    void operation_filepath_contain_fail_1();

    // FileExtensionMode

    void operation_extension_match_success_1();

    // FileSizeMode
    void operation_size_less_than_success_1();
    void operation_size_equal_success_1();
    void operation_size_equal_or_lesser_than_success_1();
    void operation_size_equal_or_greater_than_success_1();
    void operation_size_greater_than_success_1();
    void operation_size_interval_success_1();

private:
    ICoreApplication *mApp;
};
Core_functionality::Core_functionality()
{
    // Setup the core module
    mApp = new MainApplication("MHTest","MH");
}

Core_functionality::~Core_functionality()
{

}

void Core_functionality::cleanup()
{
    // Clear rules
    mApp->clearRules();
}

void Core_functionality::cleanupTestCase()
{
    // Clear the registry
    QSettings s("MH","MHTest");
    s.clear();
}

void Core_functionality::persistence_SettingsManager_Success_1()
{
    // Pre state variables
    SettingsDelegate preSettings;

    preSettings.closeOnExit = true;
    preSettings.rulesEnabled = false;
    preSettings.ruleCountInterval = 15;
    preSettings.ruleTimerEnabled = true;

    mApp->setSettings(preSettings);
    delete mApp;

    mApp = new MainApplication("MHTest","MH");

    SettingsDelegate postSettings = mApp->settingsState();

    bool isTrue = true;

    if(preSettings.closeOnExit != postSettings.closeOnExit ||
            preSettings.rulesEnabled != postSettings.rulesEnabled ||
            preSettings.ruleTimerEnabled != postSettings.ruleTimerEnabled ||
            preSettings.ruleCountInterval != postSettings.ruleCountInterval)
        isTrue = false;

    QVERIFY(isTrue);
}

void Core_functionality::persistence_SettingsManager_Success_2()
{
    // Pre state variables
    SettingsDelegate preSettings;
    preSettings.closeOnExit = true;
    preSettings.rulesEnabled = true;
    preSettings.ruleTimerEnabled = false;
    preSettings.ruleCountInterval = 0;

    mApp->setSettings(preSettings);
    delete mApp;

    mApp = new MainApplication("MHTest","MH");

    SettingsDelegate postSettings = mApp->settingsState();

    bool isTrue = true;

    if(preSettings.closeOnExit != postSettings.closeOnExit ||
            preSettings.rulesEnabled != postSettings.rulesEnabled ||
            preSettings.ruleTimerEnabled != postSettings.ruleTimerEnabled ||
            preSettings.ruleCountInterval != postSettings.ruleCountInterval)
        isTrue = false;

    QVERIFY(isTrue);
}



void Core_functionality::insert_rule_filepath_match_success_1()
{
    // Pre-state variables

    QString preAPath = "/testpath", preTitle = "Test1";
    QStringList prekWrds = QStringList() << "T1" << "T2";
    rD::fileConditionEntity preCond = rD::filepathMode;
    rD::fileCompareEntity preComp = rD::match;
    Rule preRule;

    // Initialize pre-state

    preRule.title = preTitle;
    SubRule sR;
    preRule.appliesToPath = preAPath;
    sR.keyWords = prekWrds;
    sR.fieldCondition = preCond;
    sR.fileCompareMode = preComp;

    preRule.subRules << sR;

    mApp->insertRule(preRule);

    // Post section

    Rule postRule = mApp->rule(preTitle);
    QVERIFY(RuleTools::RuleEquals(preRule,postRule));
}

void Core_functionality::insert_rule_filepath_match_fail_1()
{
    // Pre-state variables

    QString preAPath = "/testpath", preTitle = "Test2";
    QStringList prekWrds = QStringList() << "T1" << "T2";
    rD::fileConditionEntity preCond = rD::filepathMode;
    rD::fileCompareEntity preComp = rD::match;
    Rule preRule;

    // Initialize pre-state
    SubRule sR;

    preRule.title = preTitle;
    preRule.appliesToPath = preAPath;
    sR.keyWords = prekWrds;
    sR.fieldCondition = preCond;
    sR.fileCompareMode = preComp;

    preRule.subRules << sR;

    mApp->insertRule(preRule);

    // Post section

    QString postAPath = "/testpath", postTitle = "NotInTest";
    QStringList postkWrds = QStringList() << "T1" << "T2";
    rD::fileConditionEntity postCond = rD::filepathMode;
    rD::fileCompareEntity postComp = rD::match;
    Rule compareRule;

    // Post-state
    SubRule cSR;

    compareRule.title = postTitle;
    compareRule.appliesToPath = postAPath;
    cSR.keyWords = postkWrds;
    cSR.fieldCondition = postCond;
    cSR.fileCompareMode = postComp;

    compareRule.subRules << cSR;

    Rule postRule = mApp->rule(preTitle);

    QVERIFY(!RuleTools::RuleEquals(compareRule,postRule));
}

void Core_functionality::insert_rule_filepath_match_fail_2()
{
    // Pre-state variables

    QString preAPath = "/testpath", preTitle = "Test3";
    QStringList prekWrds = QStringList() << "T1" << "T2";
    rD::fileConditionEntity preCond = rD::filepathMode;
    rD::fileCompareEntity preComp = rD::match;
    Rule preRule;

    // Initialize pre-state

    preRule.title = preTitle;
    SubRule sR;
    preRule.appliesToPath = preAPath;
    sR.keyWords = prekWrds;
    sR.fieldCondition = preCond;
    sR.fileCompareMode = preComp;

    preRule.subRules << sR;

    mApp->insertRule(preRule);

    // Post section

    QString postAPath = "/testpath", postTitle = "Test3";
    QStringList postkWrds = QStringList() << "T1" << "T2";
    rD::fileConditionEntity postCond = rD::fileExtensionMode;
    rD::fileCompareEntity postComp = rD::match;
    Rule compareRule;

    // Post-state
    SubRule cSR;

    compareRule.title = postTitle;
    compareRule.appliesToPath = postAPath;
    cSR.keyWords = postkWrds;
    cSR.fieldCondition = postCond;
    cSR.fileCompareMode = postComp;

    compareRule.subRules << cSR;

    Rule postRule = mApp->rule(preTitle);

    QVERIFY(!RuleTools::RuleEquals(compareRule,postRule));
}

void Core_functionality::insert_rule_datecreated_before_succes1()
{
    // Pre state variables
    QString title = "Date rule";
    Rule preRule;
    preRule.title = title;
    preRule.actionRuleEntity = rD::Delete;

    SubRule sR;

    myDateTime mDate;
    mDate.setDate(QDate(2017,6,3));

    sR.fieldCondition = rD::fileCreatedMode;
    sR.fileCompareMode = rD::olderThan;
    sR.date = mDate;

    preRule.subRules << sR;

    mApp->insertRule(preRule);

    Rule postRule = mApp->rule(title);

    QVERIFY(RuleTools::RuleEquals(preRule,postRule));
}

void Core_functionality::insert_rule_datecreated_after_succes1()
{
    // Pre state variables
    QString title = "Date rule2";
    Rule preRule;
    preRule.title = title;
    preRule.actionRuleEntity = rD::Delete;

    SubRule sR;

    myDateTime mDate;
    mDate.setDate(QDate(2017,6,3));

    sR.fieldCondition = rD::fileCreatedMode;
    sR.fileCompareMode = rD::youngerThan;
    sR.date = mDate;

    preRule.subRules << sR;

    mApp->insertRule(preRule);

    Rule postRule = mApp->rule(title);

    QVERIFY(RuleTools::RuleEquals(preRule,postRule));
}

void Core_functionality::insert_rule_datecreated_before_fail1()
{
    // Pre state variables
    QString title = "Date rule";
    Rule preRule;
    preRule.title = title;
    preRule.actionRuleEntity = rD::Delete;

    Rule compareRule = preRule;

    SubRule sR1,sR2;

    myDateTime originalDate,compareDate;
    originalDate.setDate(QDate(2017,6,3));
    compareDate.setDate(QDate(2015,4,2));

    sR1.fieldCondition = rD::fileCreatedMode;
    sR1.fileCompareMode = rD::olderThan;
    sR1.date = originalDate;

    preRule.subRules << sR1;

    sR2.fieldCondition = rD::fileCreatedMode;
    sR2.fileCompareMode = rD::olderThan;
    sR2.date = compareDate;

    compareRule.subRules << sR2;


    mApp->insertRule(preRule);

    Rule postRule = mApp->rule(title);

    QVERIFY(!RuleTools::RuleEquals(compareRule,postRule));
}

void Core_functionality::insert_rule_datecreated_before_fail2()
{
    // Pre state variables
    QString title = "Date rule";
    Rule preRule;
    preRule.title = title;
    preRule.actionRuleEntity = rD::Delete;

    Rule compareRule = preRule;

    SubRule sR1,sR2;

    myDateTime originalDate;
    originalDate.setDate(QDate(2017,6,3));

    sR1.fieldCondition = rD::fileCreatedMode;
    sR1.fileCompareMode = rD::olderThan;
    sR1.date = originalDate;

    preRule.subRules << sR1;

    sR2.fieldCondition = rD::fileCreatedMode;
    sR2.fileCompareMode = rD::youngerThan;
    sR2.date = originalDate;

    compareRule.subRules << sR2;


    mApp->insertRule(preRule);

    Rule postRule = mApp->rule(title);

    QVERIFY(!RuleTools::RuleEquals(compareRule,postRule));
}

void Core_functionality::insert_rule_sizeinterval_success_1()
{
    // Pre state variables
    QString title = "Size interval rule 1";
    Rule preRule;
    preRule.title = title;
    preRule.actionRuleEntity = rD::Delete;

    SubRule sR;

    sR.fieldCondition = rD::fileSize;
    sR.fileCompareMode = rD::interval;

    SizeOperand minSize(244,"kb"),maxSize(512,"kb");
    sR.sizeInterval = SizeInterval(minSize,maxSize);

    preRule.subRules << sR;

    mApp->insertRule(preRule);

    Rule postRule = mApp->rule(title);

    QVERIFY(RuleTools::RuleEquals(preRule,postRule));
}

void Core_functionality::insert_rule_sizeinterval_fail_1()
{
    // Pre state variables
    QString title = "Size interval rule 2";
    Rule preRule, compareRule;
    preRule.title = title;
    preRule.actionRuleEntity = rD::Delete;

    compareRule = preRule;

    SubRule sR1,sR2;

    sR1.fieldCondition = rD::fileSize;
    sR1.fileCompareMode = rD::interval;

    sR2.fieldCondition = rD::fileSize;
    sR2.fileCompareMode = rD::interval;


    SizeOperand minSize1(244,"kb"),maxSize1(512,"kb");
    SizeOperand minSize2(384,"kb"),maxSize2(768,"kb");
    sR1.sizeInterval = SizeInterval(minSize1,maxSize1);
    sR2.sizeInterval = SizeInterval(minSize2,maxSize2);

    preRule.subRules << sR1;
    compareRule.subRules << sR2;

    mApp->insertRule(preRule);

    Rule postRule = mApp->rule(title);

    QVERIFY(!RuleTools::RuleEquals(compareRule,postRule));
}

void Core_functionality::operation_filepath_match_success_1()
{
    /*
     * INITIAL STATE:
     *  - Create dummy files in folder 'test_folder'
     */

    TestFileCreator *f_creator;
    try {
        f_creator = new  TestFileCreator();
    } catch (char *msg) {
        printf("%s\n",msg);
        QVERIFY(false);
        return;
    }

    // Pre-state variables

    const QString preTitle = "Test1";
    QStringList prekWrds = QStringList() << "Notes.txt" << "FCK.jpg";
    rD::fileActionEntity preAction = rD::Delete;
    rD::fileConditionEntity preCond = rD::filepathMode;
    rD::fileCompareEntity preComp = rD::match;
    Rule preRule;

    // Initialize pre-state

    preRule.title = preTitle;
    preRule.actionRuleEntity = preAction;

    // Create rule
    SubRule sR;
    preRule.appliesToPath = TEST_WORKING_PATH;
    sR.keyWords = prekWrds;
    sR.fieldCondition = preCond;
    sR.fileCompareMode = preComp;

    preRule.subRules << sR;

    mApp->insertRule(preRule);

    const Virtual_Objects *objects;
    try {
        objects = f_creator->createFiles(TEST_WORKING_PATH,test_file_set_1);
    } catch (const char *msg) {
        printf("%s\n",msg);
        return;
    }

    Virtual_Objects referenceList;

    // Initialize reference list with expected elements
    for (int i = 0; i < objects->count(); ++i) {
        VIRTUAL_FILE_OBJECT obj;
        try {

            obj = objects->value(i);
        }  catch (std::out_of_range *e) {
            cout << e->what() << endl;
            return Q_ASSERT(false);
        }
        QFileInfo info = obj.additionalInformation;
        bool match = false;
        for (QString str : prekWrds) {
            QString fName = info.fileName();

            if(fName == str)
               match = true;
        }
        if(!match)
            referenceList << obj;
    }

    mApp->clearFoldersAccordingToRules(mApp->watchFolders());

    /*
     * Note about async call:
     *  Due to asynchonously calls to both FileWorkOperationWorker and EntityQueueManager, tests have to wait a little amount of time
     *  to ensure proper sync between the caller and the called classes
     */

    QThread::sleep(SLEEP_SEC);

    Virtual_Objects actualList;
    try {
        actualList = f_creator->VirtualObjects(TEST_WORKING_PATH);
    }  catch (const char *msg) {
        cout << msg << endl;
    } catch (const std::domain_error *e)
    {
        cout << e->what() << endl;
        return Q_ASSERT(false);
    }


    /*
     * END STATE:
     *  - Clear test folder
     */
    int cleaned_up = false;
    try {
        cleaned_up = f_creator->emptyTestFolder(TEST_WORKING_PATH);
    } catch (const char *msg) {
        printf("%s\n",msg);
        return;
    }


    if(!cleaned_up)
        printf(DELETE_STATUS);

    QVERIFY(referenceList == actualList);
}

void Core_functionality::operation_filepath_match_fail_1()
{
    /*
     * INITIAL STATE:
     *  - Create dummy files in folder 'test_folder'
     */

    TestFileCreator *f_creator;
    try {
        f_creator = new  TestFileCreator();
    } catch (char *msg) {
        printf("%s\n",msg);
        QVERIFY(false);
        return;
    }



    // Pre-state variables

    const QString preTitle = "Test1";
    QStringList prekWrds = QStringList() << "Notes.txt" << "FCK.jpg";
    rD::fileActionEntity preAction = rD::Delete;
    rD::fileConditionEntity preCond = rD::filepathMode;
    rD::fileCompareEntity preComp = rD::match;
    Rule preRule;

    // Initialize pre-state

    preRule.title = preTitle;
    preRule.actionRuleEntity = preAction;

    // Create rule
    SubRule sR;
    preRule.appliesToPath = TEST_WORKING_PATH;
    sR.keyWords = prekWrds;
    sR.fieldCondition = preCond;
    sR.fileCompareMode = preComp;

    preRule.subRules << sR;

    mApp->insertRule(preRule);

    const Virtual_Objects *objects;
    try {
        objects = f_creator->createFiles(TEST_WORKING_PATH,test_file_set_1);
    } catch (const char *msg) {
        printf("%s\n",msg);
        return;
    }
    Q_UNUSED(objects)

    // Initialize reference list with expected/control elements
    Virtual_Objects referenceList;

    QStringList chosenList = QStringList() << prekWrds.first();

    for (int i = 0; i < objects->count(); ++i) {
        VIRTUAL_FILE_OBJECT obj = objects->value(i);
        QFileInfo info = obj.additionalInformation;
        bool match = true;
        QString fName = info.fileName();
        for (QString str : chosenList)
        {
            if(fName != str)
               match = false;
            if(!match)
                referenceList << obj;
        }
    }

    mApp->clearFoldersAccordingToRules(mApp->watchFolders());


    /*
     * Note about async call:
     *  Due to asynchonously calls to both FileWorkOperationWorker and EntityQueueManager, tests have to wait a little amount of time
     *  to ensure proper sync between the caller and the called classes
     */

    QThread::sleep(SLEEP_SEC);

    Virtual_Objects actualList;
    try {
        actualList = f_creator->VirtualObjects(TEST_WORKING_PATH);
    }  catch (const char *msg) {
        cout << msg << endl;
    } catch (const std::domain_error *e)
    {
        throw e;
    }

    /*
     * END STATE:
     *  - Clear test folder
     */
    int cleaned_up = false;
    try {
        cleaned_up = f_creator->emptyTestFolder(TEST_WORKING_PATH);
    } catch (const char *msg) {
        printf("%s\n",msg);
        return;
    }

    if(!cleaned_up)
        printf(DELETE_STATUS);

    QVERIFY(referenceList != actualList &&
            referenceList.count() == test_file_set_1.count() - chosenList.count() &&
            actualList.count() == test_file_set_1.count() - prekWrds.count());
}

void Core_functionality::operation_filepath_contain_success_1()
{

    /*
     * INITIAL STATE:
     *  - Create dummy files in folder 'test_folder'
     */

    TestFileCreator *f_creator;
    try {
        f_creator = new  TestFileCreator();
    } catch (char *msg) {
        printf("%s\n",msg);
        QVERIFY(false);
        return;
    }



    // Pre-state variables

    const QString preTitle = "Test1";
    QStringList prekWrds = QStringList() << "Notes" << "FCK";
    rD::fileActionEntity preAction = rD::Delete;
    rD::fileConditionEntity preCond = rD::filepathMode;
    rD::fileCompareEntity preComp = rD::contains;
    Rule preRule;

    // Initialize pre-state

    preRule.title = preTitle;
    preRule.actionRuleEntity = preAction;

    // Create rule
    SubRule sR;
    preRule.appliesToPath = TEST_WORKING_PATH;
    sR.keyWords = prekWrds;
    sR.fieldCondition = preCond;
    sR.fileCompareMode = preComp;

    preRule.subRules << sR;

    mApp->insertRule(preRule);

    const Virtual_Objects *objects;
    try {
        objects = f_creator->createFiles(TEST_WORKING_PATH,test_file_set_1);
    } catch (const char *msg) {
        printf("%s\n",msg);
        return;
    }

    Virtual_Objects referenceList;

    for (int i = 0; i < objects->count(); ++i) {
        VIRTUAL_FILE_OBJECT obj = objects->value(i);
        QFileInfo info = obj.additionalInformation;
        bool contains = false;
        for (QString str : prekWrds) {
            QString fName = info.fileName();

            if(fName.contains(str))
               contains = true;
        }
        if(!contains)
            referenceList << obj;
    }

    mApp->clearFoldersAccordingToRules(mApp->watchFolders());


    /*
     * Note regard async call:
     *  Due to asynchonously calls to both FileWorkOperationWorker and EntityQueueManager tests have to wait a little amount of time
     *  to ensure proper sync between the caller and the called classes
     */

    QThread::sleep(SLEEP_SEC);

    Virtual_Objects actualList;
    try {
        actualList = f_creator->VirtualObjects(TEST_WORKING_PATH);
    }  catch (const char *msg) {
        cout << msg << endl;
    } catch (const std::domain_error *e)
    {
        throw e;
    }


    /*
     * END STATE:
     *  - Clear test folder
     */
    int cleaned_up = false;
    try {
        cleaned_up = f_creator->emptyTestFolder(TEST_WORKING_PATH);
    } catch (const char *msg) {
        printf("%s\n",msg);
        return;
    }

    if(!cleaned_up)
        printf(DELETE_STATUS);

    QVERIFY(referenceList == actualList);
}

void Core_functionality::operation_filepath_contain_fail_1()
{
    /*
     * INITIAL STATE:
     *  - Create dummy files in folder 'test_folder'
     */

    TestFileCreator *f_creator;
    try {
        f_creator = new  TestFileCreator();
    } catch (char *msg) {
        printf("%s\n",msg);
        QVERIFY(false);
        return;
    }



    // Pre-state variables

    const QString preTitle = "Test1";
    QStringList prekWrds = QStringList() << "Notes" << "FCK";
    rD::fileActionEntity preAction = rD::Delete;
    rD::fileConditionEntity preCond = rD::filepathMode;
    rD::fileCompareEntity preComp = rD::contains;
    Rule preRule;

    // Initialize pre-state

    preRule.title = preTitle;
    preRule.actionRuleEntity = preAction;

    // Create rule
    SubRule sR;
    preRule.appliesToPath = TEST_WORKING_PATH;
    sR.keyWords = prekWrds;
    sR.fieldCondition = preCond;
    sR.fileCompareMode = preComp;

    preRule.subRules << sR;

    mApp->insertRule(preRule);

    const Virtual_Objects *objects;
    try {
        objects = f_creator->createFiles(TEST_WORKING_PATH,test_file_set_1);
    } catch (const char *msg) {
        printf("%s\n",msg);
        return;
    }

    Virtual_Objects referenceList;

    QStringList chosenList = QStringList() << prekWrds.first();

    for (int i = 0; i < objects->count(); ++i) {
        VIRTUAL_FILE_OBJECT obj = objects->value(i);
        QFileInfo info = obj.additionalInformation;
        QString fName = info.fileName();
        bool contains = false;
        for (QString str : chosenList) {
            if(fName.contains(str))
               contains = true;
        }
        if(!contains)
            referenceList << obj;
    }

    mApp->clearFoldersAccordingToRules(mApp->watchFolders());

    /*
     * Note regard async call:
     *  Due to asynchonously calls to both FileWorkOperationWorker and EntityQueueManager tests have to wait a little amount of time
     *  to ensure proper sync between the caller and the called classes
     */

    QThread::sleep(SLEEP_SEC);

    Virtual_Objects actualList;
    try {
        actualList = f_creator->VirtualObjects(TEST_WORKING_PATH);
    }  catch (const char *msg) {
        cout << msg << endl;
    } catch (const std::domain_error *e)
    {
        throw e;
    }


    /*
     * END STATE:
     *  - Clear test folder
     */
    int cleaned_up = false;
    try {
        cleaned_up = f_creator->emptyTestFolder(TEST_WORKING_PATH);
    } catch (const char *msg) {
        printf("%s\n",msg);
        return;
    }

    if(!cleaned_up)
        printf(DELETE_STATUS);

    QVERIFY(referenceList != actualList);
}

void Core_functionality::operation_extension_match_success_1()
{
    /*
     * INITIAL STATE:
     *  - Create dummy files in folder 'test_folder'
     */

    TestFileCreator *f_creator;
    try {
        f_creator = new  TestFileCreator();
    } catch (char *msg) {
        printf("%s\n",msg);
        QVERIFY(false);
        return;
    }



    // Pre-state variables

    const QString preTitle = "Test1";
    QStringList prekWrds = QStringList() << "txt" << "jpg";
    rD::fileActionEntity preAction = rD::Delete;
    rD::fileConditionEntity preCond = rD::fileExtensionMode;
    rD::fileCompareEntity preComp = rD::match;
    Rule preRule;

    // Initialize pre-state

    preRule.title = preTitle;
    preRule.actionRuleEntity = preAction;

    // Create rule
    SubRule sR;
    preRule.appliesToPath = TEST_WORKING_PATH;
    sR.keyWords = prekWrds;
    sR.fieldCondition = preCond;
    sR.fileCompareMode = preComp;

    preRule.subRules << sR;

    mApp->insertRule(preRule);

    const Virtual_Objects *objects;
    try {
        objects = f_creator->createFiles(TEST_WORKING_PATH,test_file_set_1);
    } catch (const char *msg) {
        printf("%s\n",msg);
        return;
    }

    Virtual_Objects referenceList;

    for (int i = 0; i < objects->count(); ++i) {
        VIRTUAL_FILE_OBJECT obj = objects->value(i);
        QFileInfo info = obj.additionalInformation;
        bool match = false;
        for (QString str : prekWrds) {
            QString suf = info.suffix();
            match = (suf == str) ?  true : match;
        }
        if(!match)
            referenceList << obj;
    }

    mApp->clearFoldersAccordingToRules(mApp->watchFolders());

    /*
     * Note regard async call:
     *  Due to asynchonously calls to both FileWorkOperationWorker and EntityQueueManager tests have to wait a little amount of time
     *  to ensure proper sync between the caller and the called classes
     */

    QThread::sleep(SLEEP_SEC);

    Virtual_Objects actualList;
    try {
        actualList = f_creator->VirtualObjects(TEST_WORKING_PATH);
    }  catch (const char *msg) {
        cout << msg << endl;
    } catch (const std::domain_error *e)
    {
        throw e;
    }


    /*
     * END STATE:
     *  - Clear test folder
     */
    int cleaned_up = false;
    try {
        cleaned_up = f_creator->emptyTestFolder(TEST_WORKING_PATH);
    } catch (const char *msg) {
        printf("%s\n",msg);
        return;
    }

    if(!cleaned_up)
        printf("Some files/folders not deleted");

    QVERIFY(referenceList == actualList);
}

void Core_functionality::operation_size_less_than_success_1()
{
    /*
     * INITIAL STATE:
     *  - Create dummy files in folder 'test_folder'
     */

    TestFileCreator *f_creator;
    try {
        f_creator = new  TestFileCreator();
    } catch (const char *msg) {
        printf("%s\n",msg);
        QVERIFY(false);
        return;
    }



    // Pre-state variables

    const QString preTitle = "Test1";
    uint sizeUnits = 100;
    QString unit = "kb";
    const QPair<uint,QString> upperLimit(sizeUnits,unit);
    rD::fileActionEntity preAction = rD::Delete;
    rD::fileConditionEntity preCond = rD::fileSize;
    rD::fileCompareEntity preComp = rD::lesserThan;
    Rule preRule;

    // Initialize pre-state

    preRule.title = preTitle;
    preRule.actionRuleEntity = preAction;

    // Create rule
    SubRule sR;
    preRule.appliesToPath = TEST_WORKING_PATH;
    sR.fieldCondition = preCond;
    sR.fileCompareMode = preComp;
    sR.sizeLimit = upperLimit;

    preRule.subRules << sR;

    mApp->insertRule(preRule);

    const Virtual_Objects *objects;
    try {
        objects = f_creator->createFiles(TEST_WORKING_PATH,test_file_set_1);
    } catch (const char *msg) {
        printf("%s\n",msg);
        return;
    }

    Virtual_Objects referenceList;
    qint64 bytes = fW::toBytes(sR.sizeLimit.first,sR.sizeLimit.second);

    for (int i = 0; i < objects->count(); ++i) {
        VIRTUAL_FILE_OBJECT obj = objects->value(i);
        qint64 sz = obj.additionalInformation.size();
        if(sz >= bytes)
            referenceList << obj;
    }

    mApp->clearFoldersAccordingToRules(mApp->watchFolders());

    /*
     * Note regard async call:
     *  Due to asynchonously calls to both FileWorkOperationWorker and EntityQueueManager tests have to wait a little amount of time
     *  to ensure proper sync between the caller and the called classes
     */

    QThread::sleep(SLEEP_SEC);

    Virtual_Objects actualList;
    try {
        actualList = f_creator->VirtualObjects(TEST_WORKING_PATH);
    }  catch (const char *msg) {
        cout << msg << endl;
    } catch (const std::domain_error *e)
    {
        throw e;
    }

    /*
     * END STATE:
     *  - Clear test folder
     */
    int cleaned_up = false;
    try {
        cleaned_up = f_creator->emptyTestFolder(TEST_WORKING_PATH);
    } catch (const char *msg) {
        printf("%s\n",msg);
        return;
    }

    if(!cleaned_up)
        printf(DELETE_STATUS);

    QVERIFY(referenceList == actualList);
}

void Core_functionality::operation_size_equal_success_1()
{
    /*
     * INITIAL STATE:
     *  - Create dummy files in folder 'test_folder'
     */

    TestFileCreator *f_creator;
    try {
        f_creator = new  TestFileCreator();
    } catch (const char *msg) {
        printf("%s\n",msg);
        QVERIFY(false);
        return;
    }

    const Virtual_Objects *objects;
    try {
        objects = f_creator->createFiles(TEST_WORKING_PATH,test_file_set_1);
    } catch (const char *msg) {
        printf("%s\n",msg);
        return;
    }



    // Pre-state variables

    const QString preTitle = "Test1";
    rD::fileActionEntity preAction = rD::Delete;
    rD::fileConditionEntity preCond = rD::fileSize;
    rD::fileCompareEntity preComp = rD::equal;
    Rule preRule;

    QString randomElement = test_file_set_1.value(qrand() % test_file_set_1.count() + 1);

    VIRTUAL_FILE_OBJECT object = f_creator->VirtualObject(randomElement);

    qint64 sizeUnits = object.additionalInformation.size();
    QString unit = "b";
    const QPair<qint64,QString> exactLimit(sizeUnits,unit);

    // Initialize pre-state

    preRule.title = preTitle;
    preRule.actionRuleEntity = preAction;

    // Create rule
    SubRule sR;
    preRule.appliesToPath = TEST_WORKING_PATH;
    sR.fieldCondition = preCond;
    sR.fileCompareMode = preComp;
    sR.sizeLimit = exactLimit;

    preRule.subRules << sR;

    mApp->insertRule(preRule);

    Virtual_Objects referenceList;
    qint64 bytes = sR.sizeLimit.first;

    for (int i = 0; i < objects->count(); ++i) {
        VIRTUAL_FILE_OBJECT obj = objects->value(i);
        qint64 sz = obj.additionalInformation.size();
        if(sz != bytes)
            referenceList << obj;
    }

    mApp->clearFoldersAccordingToRules(mApp->watchFolders());

    /*
     * Note regard async call:
     *  Due to asynchonously calls to both FileWorkOperationWorker and EntityQueueManager tests have to wait a little amount of time
     *  to ensure proper sync between the caller and the called classes
     */

    QThread::sleep(SLEEP_SEC);

    Virtual_Objects actualList;
    try {
        actualList = f_creator->VirtualObjects(TEST_WORKING_PATH);
    }  catch (const char *msg) {
        cout << msg << endl;
    } catch (const std::domain_error *e)
    {
        throw e;
    }

    /*
     * END STATE:
     *  - Clear test folder
     */
    int cleaned_up = false;
    try {
        cleaned_up = f_creator->emptyTestFolder(TEST_WORKING_PATH);
    } catch (const char *msg) {
        printf("%s\n",msg);
        return;
    }

    if(!cleaned_up)
        printf(DELETE_STATUS);

    QVERIFY(referenceList == actualList);
}

void Core_functionality::operation_size_equal_or_lesser_than_success_1()
{
    /*
     * INITIAL STATE:
     *  - Create dummy files in folder 'test_folder'
     */

    TestFileCreator *f_creator;
    try {
        f_creator = new  TestFileCreator();
    } catch (const char *msg) {
        printf("%s\n",msg);
        QVERIFY(false);
        return;
    }
    Virtual_Objects * objects = nullptr;

    try {
       objects = f_creator->createFiles(TEST_WORKING_PATH,test_file_set_1);
    }  catch (const char *msg) {
        cout << msg << endl;
        Q_ASSERT(false);
    }



    // Pre-state variables

    const QString preTitle = "Test1";
    uint sizeUnits = 100;
    QString unit = "kb";
    const QPair<uint,QString> lowerLimit(sizeUnits,unit);
    rD::fileActionEntity preAction = rD::Delete;
    rD::fileConditionEntity preCond = rD::fileSize;
    rD::fileCompareEntity preComp = rD::lesserOrEqualThan;
    Rule preRule;

    // Initialize pre-state

    preRule.title = preTitle;
    preRule.actionRuleEntity = preAction;

    // Create rule
    SubRule sR;
    preRule.appliesToPath = TEST_WORKING_PATH;
    sR.fieldCondition = preCond;
    sR.fileCompareMode = preComp;
    sR.sizeLimit = lowerLimit;

    preRule.subRules << sR;

    mApp->insertRule(preRule);


    Virtual_Objects referenceList;
    qint64 bytes = fW::toBytes(sR.sizeLimit.first,sR.sizeLimit.second);

    for (int i = 0; i < objects->count(); ++i) {
        VIRTUAL_FILE_OBJECT obj = objects->value(i);
        qint64 sz = obj.additionalInformation.size();
        if(sz > bytes)
            referenceList << obj;
    }

    mApp->clearFoldersAccordingToRules(mApp->watchFolders());

    /*
     * Note regard async call:
     *  Due to asynchonously calls to both FileWorkOperationWorker and EntityQueueManager tests have to wait a little amount of time
     *  to ensure proper sync between the caller and the called classes
     */

    QThread::sleep(SLEEP_SEC);

    Virtual_Objects actualList;
    try {
        actualList = f_creator->VirtualObjects(TEST_WORKING_PATH);
    }  catch (const char *msg) {
        cout << msg << endl;
    } catch (const std::domain_error *e)
    {
        throw e;
    }

    /*
     * END STATE:
     *  - Clear test folder
     */
    int cleaned_up = false;
    try {
        cleaned_up = f_creator->emptyTestFolder(TEST_WORKING_PATH);
    } catch (const char *msg) {
        printf("%s\n",msg);
        return;
    }

    if(!cleaned_up)
        printf(DELETE_STATUS);

    QVERIFY(referenceList == actualList);
}

void Core_functionality::operation_size_equal_or_greater_than_success_1()
{
    /*
     * INITIAL STATE:
     *  - Create dummy files in folder 'test_folder'
     */

    TestFileCreator *f_creator;
    try {
        f_creator = new  TestFileCreator();
    } catch (const char *msg) {
        printf("%s\n",msg);
        QVERIFY(false);
        return;
    }



    // Pre-state variables

    const QString preTitle = "Test1";
    uint sizeUnits = 100;
    QString unit = "kb";
    const QPair<uint,QString> lowerLimit(sizeUnits,unit);
    rD::fileActionEntity preAction = rD::Delete;
    rD::fileConditionEntity preCond = rD::fileSize;
    rD::fileCompareEntity preComp = rD::greaterOrEqualThan;
    Rule preRule;

    // Initialize pre-state

    preRule.title = preTitle;
    preRule.actionRuleEntity = preAction;

    // Create rule
    SubRule sR;
    preRule.appliesToPath = TEST_WORKING_PATH;
    sR.fieldCondition = preCond;
    sR.fileCompareMode = preComp;
    sR.sizeLimit = lowerLimit;

    preRule.subRules << sR;

    mApp->insertRule(preRule);

    const Virtual_Objects *objects;
    try {
        objects = f_creator->createFiles(TEST_WORKING_PATH,test_file_set_1);
    } catch (const char *msg) {
        printf("%s\n",msg);
        return;
    }

    Virtual_Objects referenceList;
    qint64 bytes = fW::toBytes(sR.sizeLimit.first,sR.sizeLimit.second);

    for (int i = 0; i < objects->count(); ++i) {
        VIRTUAL_FILE_OBJECT obj = objects->value(i);
        qint64 sz = obj.additionalInformation.size();
        if(sz < bytes)
            referenceList << obj;
    }

    mApp->clearFoldersAccordingToRules(mApp->watchFolders());

    /*
     * Note regard async call:
     *  Due to asynchonously calls to both FileWorkOperationWorker and EntityQueueManager tests have to wait a little amount of time
     *  to ensure proper sync between the caller and the called classes
     */

    QThread::sleep(SLEEP_SEC);

    Virtual_Objects actualList;
    try {
        actualList = f_creator->VirtualObjects(TEST_WORKING_PATH);
    }  catch (const char *msg) {
        cout << msg << endl;
    } catch (const std::domain_error *e)
    {
        throw e;
    }

    /*
     * END STATE:
     *  - Clear test folder
     */
    int cleaned_up = false;
    try {
        cleaned_up = f_creator->emptyTestFolder(TEST_WORKING_PATH);
    } catch (const char *msg) {
        printf("%s\n",msg);
        return;
    }

    if(!cleaned_up)
        printf(DELETE_STATUS);

    QVERIFY(referenceList == actualList);
}

void Core_functionality::operation_size_greater_than_success_1()
{
    /*
     * INITIAL STATE:
     *  - Create dummy files in folder 'test_folder'
     */

    TestFileCreator *f_creator;
    try {
        f_creator = new  TestFileCreator();
    } catch (const char *msg) {
        printf("%s\n",msg);
        QVERIFY(false);
        return;
    }



    // Pre-state variables

    const QString preTitle = "Test1";
    uint sizeUnits = 100;
    QString unit = "kb";
    const QPair<uint,QString> lowerLimit(sizeUnits,unit);
    rD::fileActionEntity preAction = rD::Delete;
    rD::fileConditionEntity preCond = rD::fileSize;
    rD::fileCompareEntity preComp = rD::greaterThan;
    Rule preRule;

    // Initialize pre-state

    preRule.title = preTitle;
    preRule.actionRuleEntity = preAction;

    // Create rule
    SubRule sR;
    preRule.appliesToPath = TEST_WORKING_PATH;
    sR.fieldCondition = preCond;
    sR.fileCompareMode = preComp;
    sR.sizeLimit = lowerLimit;

    preRule.subRules << sR;

    mApp->insertRule(preRule);
    const Virtual_Objects *objects;
    try {
        objects = f_creator->createFiles(TEST_WORKING_PATH,test_file_set_1);
    } catch (const char *msg) {
        printf("%s\n",msg);
        return;
    }

    Virtual_Objects referenceList;
    qint64 bytes = fW::toBytes(sR.sizeLimit.first,sR.sizeLimit.second);

    for (int i = 0; i < objects->count(); ++i) {
        VIRTUAL_FILE_OBJECT obj;
        try {
            obj = objects->value(i);
        }  catch (std::out_of_range *e) {
            cout << e->what() << endl;
            return Q_ASSERT(false);
        }
        qint64 sz = obj.additionalInformation.size();
        if(sz <= bytes)
            referenceList << obj;
    }

    mApp->clearFoldersAccordingToRules(mApp->watchFolders());

    /*
     * Note regard async call:
     *  Due to asynchonously calls to both FileWorkOperationWorker and EntityQueueManager tests have to wait a little amount of time
     *  to ensure proper sync between the caller and the called classes
     */

    QThread::sleep(SLEEP_SEC);

    Virtual_Objects actualList;
    try {
        actualList = f_creator->VirtualObjects(TEST_WORKING_PATH);
    }  catch (const char *msg) {
        cout << msg << endl;
    } catch (const std::domain_error *e)
    {
        cout << e->what() << endl;
        return Q_ASSERT(false);
    }

    /*
     * END STATE:
     *  - Clear test folder
     */
    int cleaned_up = false;
    try {
        cleaned_up = f_creator->emptyTestFolder(TEST_WORKING_PATH);
    } catch (const char *msg) {
        printf("%s\n",msg);
        return;
    }

    if(!cleaned_up)
        printf(DELETE_STATUS);

    QVERIFY(referenceList == actualList);
}

void Core_functionality::operation_size_interval_success_1()
{
    /*
     * INITIAL STATE:
     *  - Create dummy files in folder 'test_folder'
     */

    TestFileCreator *f_creator;
    try {
        f_creator = new  TestFileCreator();
    } catch (const char *msg) {
        printf("%s\n",msg);
        QVERIFY(false);
        return;
    }

    const Virtual_Objects *objects;
    try {
        objects = f_creator->createFiles(TEST_WORKING_PATH,test_file_set_1);
    } catch (const char *msg) {
        printf("%s\n",msg);
        return;
    }

    // Pre-state variables

    const QString preTitle = "Test1";
    rD::fileActionEntity preAction = rD::Delete;
    rD::fileConditionEntity preCond = rD::fileSize;
    rD::fileCompareEntity preComp = rD::interval;
    SizeLimits interval;
    IntervalUnit lowerLimit, upperLimit;

    lowerLimit.first = 60;
    lowerLimit.second = "kb";

    upperLimit.first = 125;
    upperLimit.second = "kb";

    interval.first = lowerLimit;
    interval.second = upperLimit;

    Rule preRule;

    // Initialize pre-state

    preRule.title = preTitle;
    preRule.actionRuleEntity = preAction;

    // Create rule
    SubRule sR;
    preRule.appliesToPath = TEST_WORKING_PATH;
    sR.fieldCondition = preCond;
    sR.fileCompareMode = preComp;
    sR.sizeInterval = interval;

    preRule.subRules << sR;

    mApp->insertRule(preRule);


    Virtual_Objects referenceList;

    for (int i = 0; i < objects->count(); ++i) {
        VIRTUAL_FILE_OBJECT obj = objects->value(i);
        qint64 sz = obj.additionalInformation.size();
        if(sz <= fW::toBytes(lowerLimit.first,lowerLimit.second) || sz >= fW::toBytes(upperLimit.first,lowerLimit.second))
            referenceList << obj;
    }

    mApp->clearFoldersAccordingToRules(mApp->watchFolders());

    /*
     * Note regard async call:
     *  Due to asynchonously calls to both FileWorkOperationWorker and EntityQueueManager tests have to wait a little amount of time
     *  to ensure proper sync between the caller and the called classes
     */

    QThread::sleep(SLEEP_SEC);

    Virtual_Objects actualList;
    try {
        actualList = f_creator->VirtualObjects(TEST_WORKING_PATH);
    }  catch (const char *msg) {
        cout << msg << endl;
    } catch (const std::domain_error *e)
    {
        cout << e->what() << endl;
        return Q_ASSERT(false);
    }

    /*
     * END STATE:
     *  - Clear test folder
     */
    int cleaned_up = false;
    try {
        cleaned_up = f_creator->emptyTestFolder(TEST_WORKING_PATH);
    } catch (const char *msg) {
        printf("%s\n",msg);
        return;
    }

    if(!cleaned_up)
        printf(DELETE_STATUS);

    QVERIFY(referenceList == actualList);
}

    QTEST_MAIN(Core_functionality)

#include "tst_core_functionality.moc"
#endif
