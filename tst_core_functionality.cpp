#include "applicationdomain.h"
#include "rulescontext.h"
#include "testfilecreator.h"
#include "rulebuilder.h"
#include "defaultRuleConfiguration.h"

#ifdef TEST_MODE
#include <QtTest>
#define SLEEP_SEC 1
#define DELETE_STATUS "Some files/folders not deleted"

#ifdef __WIN64__
    #define TEST_WORKING_PATH QDir::homePath() + "/test_folder/"
    #define TEST_SECONDARY_PATH QDir::homePath() + "/test_folder_copy"
#elif __GNUC__
    #define TEST_WORKING_PATH QDir::homePath() + "/MyDocs/Programming/Test/FileMover_Test_Folder/"
    #define TEST_SECONDARY_PATH QDir::homePath() + "/MyDocs/Programming/Test/FileMover_New_Test_Folder/"
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

    // Filepath mode
    void operation_filepath_match_success_1();
    void operation_filepath_match_fail_1();
    void operation_filepath_contain_success_1();
    void operation_filepath_contain_fail_1();

    // File extension mode

    void operation_extension_match_success_1();

    // Filesize mode
    void operation_size_less_than_success_1();
    void operation_size_equal_success_1();
    void operation_size_equal_or_lesser_than_success_1();
    void operation_size_equal_or_greater_than_success_1();
    void operation_size_greater_than_success_1();
    void operation_size_interval_success_1();

    /*
     * Move
     */

    // Filepath Mode

    void operation_move_filepath_match_success_1();

private:
    bool testPersistence(bool rulesEnabled,bool ruleTimerEnabled, bool closeOnExitEnabled, int ruleTimerInterval)
    {
        // Pre state variables

        mApp->setSettings(closeOnExitEnabled,ruleTimerEnabled,rulesEnabled,QRect(),ruleTimerInterval);
        delete mApp;

        mApp = new ApplicationDomain();

        auto postSettings = mApp->settingsState();

        return closeOnExitEnabled == postSettings->closeOnExit() &&
                ruleTimerEnabled == postSettings->rulesEnabled() &&
                rulesEnabled == postSettings->ruleTimerEnabled() &&
                 ruleTimerInterval == postSettings->ruleCountInterval();
    }

    bool testInsertRule(IDefaultRuleConfigurator *ruleConfig,
                        IDefaultConditionConfigurator * ruleConditionConfig)
    {
        // Pre-state variables

        auto ruleBuilder = new RuleBuilder();
        auto rule = ruleBuilder->buildRule(ruleConfig,QList<const IDefaultConditionConfigurator*>() << ruleConditionConfig);


        // Initialize pre-state


        mApp->insertRule(rule);

        // Post section

        auto postRule = mApp->rule(ruleConfig->title());
    }

    AbstractApplicationService *mApp;
};
Core_functionality::Core_functionality()
{
    mApp = new ApplicationDomain();
    mApp->addWatchFolder(TEST_WORKING_PATH);
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

    QVERIFY(testPersistence(false,true,true,15));
}

void Core_functionality::persistence_SettingsManager_Success_2()
{
    QVERIFY(testPersistence(false,true,true,0));
}



void Core_functionality::insert_rule_filepath_match_success_1()
{
    // Pre-state variables

    QString preAPath = "/testpath", preTitle = "Test1";
    QStringList prekWrds = QStringList() << "T1" << "T2";
    RulesContext::RuleCriteria preCond = RulesContext::FileNameMode;
    RulesContext::RuleCompareCriteria preComp = RulesContext::match;
    Rule preRule;

    // Initialize pre-state

    preRule.title = preTitle;
    SubRule sR;
    preRule.appliesToPath = preAPath;
    sR.keyWords = prekWrds;
    sR.criteria = preCond;
    sR.compareCriteria = preComp;

    preRule.subRules << sR;

    mApp->insertRule(preRule);

    // Post section

    Rule postRule = mApp->rule(preTitle);
    QVERIFY(TestRuleHelper::RuleEquals(preRule,postRule));
}

void Core_functionality::insert_rule_filepath_match_fail_1()
{
    // Pre-state variables

    QString preAPath = "/testpath", preTitle = "Test2";
    QStringList prekWrds = QStringList() << "T1" << "T2";
    RulesContext::RuleCriteria preCond = RulesContext::FileNameMode;
    RulesContext::RuleCompareCriteria preComp = RulesContext::match;
    Rule preRule;

    // Initialize pre-state
    SubRule sR;

    preRule.title = preTitle;
    preRule.appliesToPath = preAPath;
    sR.keyWords = prekWrds;
    sR.criteria = preCond;
    sR.compareCriteria = preComp;

    preRule.subRules << sR;

    mApp->insertRule(preRule);

    // Post section

    QString postAPath = "/testpath", postTitle = "NotInTest";
    QStringList postkWrds = QStringList() << "T1" << "T2";
    RulesContext::RuleCriteria postCond = RulesContext::FileNameMode;
    RulesContext::RuleCompareCriteria postComp = RulesContext::match;
    Rule compareRule;

    // Post-state
    SubRule cSR;

    compareRule.title = postTitle;
    compareRule.appliesToPath = postAPath;
    cSR.keyWords = postkWrds;
    cSR.criteria = postCond;
    cSR.compareCriteria = postComp;

    compareRule.subRules << cSR;

    Rule postRule = mApp->rule(preTitle);

    QVERIFY(!TestRuleHelper::RuleEquals(compareRule,postRule));
}

void Core_functionality::insert_rule_filepath_match_fail_2()
{
    // Pre-state variables

    QString preAPath = "/testpath", preTitle = "Test3";
    QStringList prekWrds = QStringList() << "T1" << "T2";
    RulesContext::RuleCriteria preCond = RulesContext::FileNameMode;
    RulesContext::RuleCompareCriteria preComp = RulesContext::match;
    Rule preRule;

    // Initialize pre-state

    preRule.title = preTitle;
    SubRule sR;
    preRule.appliesToPath = preAPath;
    sR.keyWords = prekWrds;
    sR.criteria = preCond;
    sR.compareCriteria = preComp;

    preRule.subRules << sR;

    mApp->insertRule(preRule);

    // Post section

    QString postAPath = "/testpath", postTitle = "Test3";
    QStringList postkWrds = QStringList() << "T1" << "T2";
    RulesContext::RuleCriteria postCond = RulesContext::fileExtensionMode;
    RulesContext::RuleCompareCriteria postComp = RulesContext::match;
    Rule compareRule;

    // Post-state
    SubRule cSR;

    compareRule.title = postTitle;
    compareRule.appliesToPath = postAPath;
    cSR.keyWords = postkWrds;
    cSR.criteria = postCond;
    cSR.compareCriteria = postComp;

    compareRule.subRules << cSR;

    Rule postRule = mApp->rule(preTitle);

    QVERIFY(!TestRuleHelper::RuleEquals(compareRule,postRule));
}

void Core_functionality::insert_rule_datecreated_before_succes1()
{
    // Pre state variables
    QString title = "Date rule";
    Rule preRule;
    preRule.title = title;
    preRule.actionRuleEntity = RulesContext::Delete;

    SubRule sR;

    myDateTime mDate;
    mDate.setDate(QDate(2017,6,3));

    sR.criteria = RulesContext::fileCreatedMode;
    sR.compareCriteria = RulesContext::olderThan;
    sR.date = mDate;

    preRule.subRules << sR;

    mApp->insertRule(preRule);

    Rule postRule = mApp->rule(title);

    QVERIFY(TestRuleHelper::RuleEquals(preRule,postRule));
}

void Core_functionality::insert_rule_datecreated_after_succes1()
{
    // Pre state variables
    QString title = "Date rule2";
    Rule preRule;
    preRule.title = title;
    preRule.actionRuleEntity = RulesContext::Delete;

    SubRule sR;

    myDateTime mDate;
    mDate.setDate(QDate(2017,6,3));

    sR.criteria = RulesContext::fileCreatedMode;
    sR.compareCriteria = RulesContext::youngerThan;
    sR.date = mDate;

    preRule.subRules << sR;

    mApp->insertRule(preRule);

    Rule postRule = mApp->rule(title);

    QVERIFY(TestRuleHelper::RuleEquals(preRule,postRule));
}

void Core_functionality::insert_rule_datecreated_before_fail1()
{
    // Pre state variables
    QString title = "Date rule";
    Rule preRule;
    preRule.title = title;
    preRule.actionRuleEntity = RulesContext::Delete;

    Rule compareRule = preRule;

    SubRule sR1,sR2;

    myDateTime originalDate,compareDate;
    originalDate.setDate(QDate(2017,6,3));
    compareDate.setDate(QDate(2015,4,2));

    sR1.criteria = RulesContext::fileCreatedMode;
    sR1.compareCriteria = RulesContext::olderThan;
    sR1.date = originalDate;

    preRule.subRules << sR1;

    sR2.criteria = RulesContext::fileCreatedMode;
    sR2.compareCriteria = RulesContext::olderThan;
    sR2.date = compareDate;

    compareRule.subRules << sR2;


    mApp->insertRule(preRule);

    Rule postRule = mApp->rule(title);

    QVERIFY(!TestRuleHelper::RuleEquals(compareRule,postRule));
}

void Core_functionality::insert_rule_datecreated_before_fail2()
{
    // Pre state variables
    QString title = "Date rule";
    Rule preRule;
    preRule.title = title;
    preRule.actionRuleEntity = RulesContext::Delete;

    Rule compareRule = preRule;

    SubRule sR1,sR2;

    myDateTime originalDate;
    originalDate.setDate(QDate(2017,6,3));

    sR1.criteria = RulesContext::fileCreatedMode;
    sR1.compareCriteria = RulesContext::olderThan;
    sR1.date = originalDate;

    preRule.subRules << sR1;

    sR2.criteria = RulesContext::fileCreatedMode;
    sR2.compareCriteria = RulesContext::youngerThan;
    sR2.date = originalDate;

    compareRule.subRules << sR2;


    mApp->insertRule(preRule);

    Rule postRule = mApp->rule(title);

    QVERIFY(!TestRuleHelper::RuleEquals(compareRule,postRule));
}

void Core_functionality::insert_rule_sizeinterval_success_1()
{
    // Pre state variables
    QString title = "Size interval rule 1";
    Rule preRule;
    preRule.title = title;
    preRule.actionRuleEntity = RulesContext::Delete;

    SubRule sR;

    sR.criteria = RulesContext::fileSize;
    sR.compareCriteria = RulesContext::interval;

    SizeOperand minSize(244,"kb"),maxSize(512,"kb");
    sR.sizeInterval = SizeInterval(minSize,maxSize);

    preRule.subRules << sR;

    mApp->insertRule(preRule);

    Rule postRule = mApp->rule(title);

    QVERIFY(TestRuleHelper::RuleEquals(preRule,postRule));
}

void Core_functionality::insert_rule_sizeinterval_fail_1()
{
    // Pre state variables
    QString title = "Size interval rule 2";
    Rule preRule, compareRule;
    preRule.title = title;
    preRule.actionRuleEntity = RulesContext::Delete;

    compareRule = preRule;

    SubRule sR1,sR2;

    sR1.criteria = RulesContext::fileSize;
    sR1.compareCriteria = RulesContext::interval;

    sR2.criteria = RulesContext::fileSize;
    sR2.compareCriteria = RulesContext::interval;


    SizeOperand minSize1(244,"kb"),maxSize1(512,"kb");
    SizeOperand minSize2(384,"kb"),maxSize2(768,"kb");
    sR1.sizeInterval = SizeInterval(minSize1,maxSize1);
    sR2.sizeInterval = SizeInterval(minSize2,maxSize2);

    preRule.subRules << sR1;
    compareRule.subRules << sR2;

    mApp->insertRule(preRule);

    Rule postRule = mApp->rule(title);

    QVERIFY(!TestRuleHelper::RuleEquals(compareRule,postRule));
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

    QStringList prekWrds = QStringList() << "Notes.txt" << "FCK.jpg";

    const Virtual_Objects *objects;
    try {
        objects = initialize_pre_state(RulesContext::Delete,
                                       RulesContext::FileNameMode,
                                       RulesContext::match,
                                       prekWrds,
                                       TEST_WORKING_PATH,
                                       f_creator);
    } catch (const char *msg) {
        printf("%s\n",msg);
        return;
    }

    Virtual_Objects referenceList;

    // Initialize reference list with expected elements
    for (int i = 0; i < objects->count(); ++i) {
        VIRTUAL_FILE_OBJECT obj;
        try {

            obj = objects->getVirtualObjectFromIndex(i);
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

    // FILTER STATE:

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

    QStringList prekWrds = QStringList() << "Notes.txt" << "FCK.jpg";

    const Virtual_Objects *objects;
    try {
        objects = initialize_pre_state(RulesContext::Delete,
                                       RulesContext::FileNameMode,
                                       RulesContext::match,
                                       prekWrds,
                                       TEST_WORKING_PATH,
                                       f_creator);
    } catch (const char *msg) {
        printf("%s\n",msg);
        return;
    }

    // Initialize reference list with expected/control elements
    Virtual_Objects referenceList;

    QStringList chosenList = QStringList() << prekWrds.first();

    for (int i = 0; i < objects->count(); ++i) {
        VIRTUAL_FILE_OBJECT obj = objects->getVirtualObjectFromIndex(i);
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

    QStringList prekWrds = QStringList() << "Notes.txt" << "FCK.jpg";

    const Virtual_Objects *objects;
    try {
        objects = initialize_pre_state(RulesContext::Delete,
                                       RulesContext::FileNameMode,
                                       RulesContext::contains,
                                       prekWrds,
                                       TEST_WORKING_PATH,
                                       f_creator);
    } catch (const char *msg) {
        printf("%s\n",msg);
        return;
    }

    Virtual_Objects referenceList;

    for (int i = 0; i < objects->count(); ++i) {
        VIRTUAL_FILE_OBJECT obj = objects->getVirtualObjectFromIndex(i);
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

    QStringList prekWrds = QStringList() << "Notes.txt" << "FCK.jpg";

    const Virtual_Objects *objects;
    try {
        objects = initialize_pre_state(RulesContext::Delete,
                                       RulesContext::FileNameMode,
                                       RulesContext::contains,
                                       prekWrds,
                                       TEST_WORKING_PATH,
                                       f_creator);
    } catch (const char *msg) {
        printf("%s\n",msg);
        return;
    }

    Virtual_Objects referenceList;

    QStringList chosenList = QStringList() << prekWrds.first();

    for (int i = 0; i < objects->count(); ++i) {
        VIRTUAL_FILE_OBJECT obj = objects->getVirtualObjectFromIndex(i);
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
    RulesContext::RuleAction preAction = RulesContext::DeleteAction;
    RulesContext::RuleCriteria preCond = RulesContext::FileExtensionMode;
    RulesContext::RuleCompareCriteria preComp = RulesContext::Match;
    Rule preRule;

    // Initialize pre-state

    preRule.title = preTitle;
    preRule.actionRuleEntity = preAction;

    // Create rule
    SubRule sR;
    preRule.appliesToPath = TEST_WORKING_PATH;
    sR.keyWords = prekWrds;
    sR.criteria = preCond;
    sR.compareCriteria = preComp;

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
        VIRTUAL_FILE_OBJECT obj = objects->getVirtualObjectFromIndex(i);
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
    RulesContext::RuleAction preAction = RulesContext::Delete;
    RulesContext::RuleCriteria preCond = RulesContext::fileSize;
    RulesContext::RuleCompareCriteria preComp = RulesContext::lesserThan;
    Rule preRule;

    // Initialize pre-state

    preRule.title = preTitle;
    preRule.actionRuleEntity = preAction;

    // Create rule
    SubRule sR;
    preRule.appliesToPath = TEST_WORKING_PATH;
    sR.criteria = preCond;
    sR.compareCriteria = preComp;
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
    qint64 bytes = SBC::convertToBytes(sR.sizeLimit.first,sR.sizeLimit.second);

    for (int i = 0; i < objects->count(); ++i) {
        VIRTUAL_FILE_OBJECT obj = objects->getVirtualObjectFromIndex(i);
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
    RulesContext::RuleAction preAction = RulesContext::Delete;
    RulesContext::RuleCriteria preCond = RulesContext::fileSize;
    RulesContext::RuleCompareCriteria preComp = RulesContext::equal;
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
    sR.criteria = preCond;
    sR.compareCriteria = preComp;
    sR.sizeLimit = exactLimit;

    preRule.subRules << sR;

    mApp->insertRule(preRule);

    Virtual_Objects referenceList;
    qint64 bytes = sR.sizeLimit.first;

    for (int i = 0; i < objects->count(); ++i) {
        VIRTUAL_FILE_OBJECT obj = objects->getVirtualObjectFromIndex(i);
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
    RulesContext::RuleAction preAction = RulesContext::Delete;
    RulesContext::RuleCriteria preCond = RulesContext::fileSize;
    RulesContext::RuleCompareCriteria preComp = RulesContext::lesserOrEqualThan;
    Rule preRule;

    // Initialize pre-state

    preRule.title = preTitle;
    preRule.actionRuleEntity = preAction;

    // Create rule
    SubRule sR;
    preRule.appliesToPath = TEST_WORKING_PATH;
    sR.criteria = preCond;
    sR.compareCriteria = preComp;
    sR.sizeLimit = lowerLimit;

    preRule.subRules << sR;

    mApp->insertRule(preRule);


    Virtual_Objects referenceList;
    qint64 bytes = SBC::convertToBytes(sR.sizeLimit.first,sR.sizeLimit.second);

    for (int i = 0; i < objects->count(); ++i) {
        VIRTUAL_FILE_OBJECT obj = objects->getVirtualObjectFromIndex(i);
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
    RulesContext::RuleAction preAction = RulesContext::Delete;
    RulesContext::RuleCriteria preCond = RulesContext::fileSize;
    RulesContext::RuleCompareCriteria preComp = RulesContext::greaterOrEqualThan;
    Rule preRule;

    // Initialize pre-state

    preRule.title = preTitle;
    preRule.actionRuleEntity = preAction;

    // Create rule
    SubRule sR;
    preRule.appliesToPath = TEST_WORKING_PATH;
    sR.criteria = preCond;
    sR.compareCriteria = preComp;
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
    qint64 bytes = SBC::convertToBytes(sR.sizeLimit.first,sR.sizeLimit.second);

    for (int i = 0; i < objects->count(); ++i) {
        VIRTUAL_FILE_OBJECT obj = objects->getVirtualObjectFromIndex(i);
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
    RulesContext::RuleAction preAction = RulesContext::Delete;
    RulesContext::RuleCriteria preCond = RulesContext::fileSize;
    RulesContext::RuleCompareCriteria preComp = RulesContext::greaterThan;
    Rule preRule;

    // Initialize pre-state

    preRule.title = preTitle;
    preRule.actionRuleEntity = preAction;

    // Create rule
    SubRule sR;
    preRule.appliesToPath = TEST_WORKING_PATH;
    sR.criteria = preCond;
    sR.compareCriteria = preComp;
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
    qint64 bytes = SBC::convertToBytes(sR.sizeLimit.first,sR.sizeLimit.second);

    for (int i = 0; i < objects->count(); ++i) {
        VIRTUAL_FILE_OBJECT obj;
        try {
            obj = objects->getVirtualObjectFromIndex(i);
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
    RulesContext::RuleAction preAction = RulesContext::Delete;
    RulesContext::RuleCriteria preCond = RulesContext::fileSize;
    RulesContext::RuleCompareCriteria preComp = RulesContext::interval;
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
    sR.criteria = preCond;
    sR.compareCriteria = preComp;
    sR.sizeInterval = interval;

    preRule.subRules << sR;

    mApp->insertRule(preRule);


    Virtual_Objects referenceList;

    for (int i = 0; i < objects->count(); ++i) {
        VIRTUAL_FILE_OBJECT obj = objects->getVirtualObjectFromIndex(i);
        qint64 sz = obj.additionalInformation.size();
        if(sz <= SBC::convertToBytes(lowerLimit.first,lowerLimit.second) || sz >= SBC::convertToBytes(upperLimit.first,lowerLimit.second))
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

void Core_functionality::operation_move_filepath_match_success_1()
{
    TestFileCreator *f_creator;
    try {
        f_creator = new TestFileCreator();
    }  catch (const char *msg) {
        printf("%s\n",msg);
        return;
    }
    QStringList prekWrds = QStringList() << "Notes.txt" << "FCK.jpg";

    const Virtual_Objects *objects;
    try {
        objects = initialize_pre_state(RulesContext::Move,
                                       RulesContext::FileNameMode,
                                       RulesContext::match,
                                       prekWrds,
                                       TEST_WORKING_PATH,
                                       f_creator,
                                       QStringList() << TEST_SECONDARY_PATH);
    } catch (const char *msg) {
        printf("%s\n",msg);
        return;
    }

    Virtual_Objects referenceList;

    // Initialize reference list with expected elements
    for (int i = 0; i < objects->count(); ++i) {
        VIRTUAL_FILE_OBJECT obj;
        try {

            obj = objects->getVirtualObjectFromIndex(i);
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




}

const Virtual_Objects *Core_functionality::initialize_pre_state(RulesContext::RuleAction ruleAction,
                                                                RulesContext::RuleCriteria criteria,
                                                                RulesContext::RuleCompareCriteria compareCriteria,
                                                                QStringList test_elements,QString filepath,
                                                                TestFileCreator *file_creator,QStringList destinations)
{
    // Pre-state variables

    const QString preTitle = "Test1";
    QStringList prekWrds = test_elements;
    RulesContext::RuleAction preAction = ruleAction;
    RulesContext::RuleCriteria preCond = criteria;
    RulesContext::RuleCompareCriteria preComp = compareCriteria;
    Rule preRule;

    // Initialize pre-state

    preRule.title = preTitle;
    preRule.actionRuleEntity = preAction;

    // Create rule
    SubRule sR;
    preRule.appliesToPath = filepath;
    sR.keyWords = prekWrds;
    sR.criteria = preCond;
    sR.compareCriteria = preComp;

    preRule.subRules << sR;
    preRule.destinationPaths = destinations;

    mApp->insertRule(preRule);

    const Virtual_Objects *objects;
    try {
        objects = file_creator->createFiles(TEST_WORKING_PATH,test_file_set_1);
    } catch (const char *msg) {
        printf("%s\n",msg);
        throw "Creating files failed.";
    }

    return objects;
}


    QTEST_MAIN(Core_functionality)

#include "tst_core_functionality.moc"
#endif
