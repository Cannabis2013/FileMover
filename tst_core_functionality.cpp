#include "applicationdomain.h"
#include "rulescontext.h"
#include "testfilecreator.h"
#include "rulebuilder.h"
#include "defaultRuleConfiguration.h"
#include "fileworker.h"
#include "rulesmanager.h"
#include "threadsmanager.h"
#include "settingsmanager.h"
#include "fileinformationmanager.h"
#include "entityqueuemanager.h"
#include "filesystemwatcher.h"
#include "filelistservice.h"
#include "ruledefinitions.h"
#include "filteringcontext.h"
#include "settingsbuilder.h"
#include "entitymodelbuilder.h"

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
    void testApplicationPersistenceSuccess1();
    void testApplicationPersistenceSuccess2();
    void testApplicationPersistenceFail1();

    /*
     * Rules section
     */

    void insert_rule_filename_match_success_1();
    void insert_rule_filename_match_succes_2();
    void insert_rule_created_before_succes1();
    void insert_rule_datecreated_after_succes1();
    void insert_rule_sizeinterval_success_1();

    /*
     * FileOperations section
     */

    /*
     * Delete
     */

    // Filepath mode
    void operationFilenameMatchSuccess1();
    void operationFilenameContainSuccess1();

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

    void operation_move_filename_match_success_1();

private:
    AbstractApplicationService *initApplicationState()
    {
        auto fileListService = (new FileListService())->setModelBuilderService(new FileModelBuilder());
        auto app = new ApplicationDomain();


        auto settingsBuilder = new SettingsBuilder();
        app->setFileOperationsService(new FileWorker())->
                setRuleManagerService(new rulesManager("TESTAPP","TESTORG",new RuleBuilder()))->
                setThreadManagerService(new ThreadsManager())->
                setSettingsManagerService(new settingsManager("TESTAPP","TESTORG",settingsBuilder,))->
                setFileInformationManagerService(new FileInformationManager("TESTAPP","TESTORG"))->
                setEntityQueueManagerService(new EntityQueueManager())->
                setFileWatcherService(new FileSystemWatcher())->
                setFileModelBuilderService(new FileListService())->
                setRuleDefinitionsService(new RuleDefinitions())->
                setFilteringContext(new FilteringContext(),fileListService)->
                configureServices()->
                startServices();

        return app;
    }

    const ISettingsModel<QRect> *initializeApplicationPersistence(bool rulesEnabled,bool ruleTimerEnabled, bool closeOnExitEnabled, int ruleTimerInterval)
    {
        // Pre state variables

        mApp->setSettings(closeOnExitEnabled,ruleTimerEnabled,rulesEnabled,QRect(),ruleTimerInterval);
        delete mApp;

        mApp = initApplicationState();

        auto postSettings = mApp->settingsState();

        auto result = closeOnExitEnabled == postSettings->isCloseOnExitEnabled() &&
                rulesEnabled == postSettings->isRulesEnabled() &&
                ruleTimerEnabled == postSettings->isRuleTimerEnabled() &&
                 ruleTimerInterval == postSettings->ruleTimerInterval();


        return postSettings;
    }

    bool testInsertRule(IDefaultRuleConfigurator *ruleConfig,
                        QList<const IDefaultConditionConfigurator *> ruleConditionConfigs)
    {
        auto ruleBuilder = new RuleBuilder();
        QList<const DefaultRuleCriteria*> criterias;
        for (auto config : ruleConditionConfigs) {
            criterias << ruleBuilder->buildCriteria(config);
        }

        auto preRule = ruleBuilder->buildRule(ruleConfig, criterias);

        // Insert rule into application domain
        mApp->insertRule(preRule);

        // Retrieve rule from application domain

        auto postRule = mApp->rule(ruleConfig->title());

        return preRule == postRule;
    }

    AbstractApplicationService *mApp;
    const Virtual_Objects *initializePreState(const IDefaultRuleConfigurator *ruleConfig, QList<const IDefaultConditionConfigurator *> ruleConditionConfigs, TestFileCreator *fileCreator);
};
Core_functionality::Core_functionality()
{

    mApp = initApplicationState();

    mApp->addWatchFolder(TEST_WORKING_PATH);
}

Core_functionality::~Core_functionality(){}

void Core_functionality::cleanup()
{
    // Clear rules
    mApp->clearRules();
    mApp->clearWatchFolders();
}

void Core_functionality::cleanupTestCase()
{
    // Clear the registry
    QSettings s("MH","MHTest");
    s.clear();
}

void Core_functionality::testApplicationPersistenceSuccess1()
{
    // Pre state variables

    bool preRulesEnabled = false;
    bool preCloseOnExit = true;
    bool preRuleTimerEnabled = true;
    int preRuleTimerInterval = 15;

    // Initialize application state
    auto applicationSettingsState = initializeApplicationPersistence(preRulesEnabled,preRuleTimerEnabled,preCloseOnExit,preRuleTimerInterval);

    bool expectedCloseOnExit = true;
    bool exprectedRulesEnabled = false;
    bool expectedRuleTimerEnabled = true;
    int expectedRuleTimerInterval = 15;


    auto result = expectedCloseOnExit == applicationSettingsState->isCloseOnExitEnabled() &&
            exprectedRulesEnabled == applicationSettingsState->isRulesEnabled()&&
            expectedRuleTimerEnabled == applicationSettingsState->isRuleTimerEnabled()&&
             expectedRuleTimerInterval == applicationSettingsState->ruleTimerInterval();

    QVERIFY(result);
}

void Core_functionality::testApplicationPersistenceSuccess2()
{
    // Pre state variables

    bool preRulesEnabled = false;
    bool preCloseOnExit = true;
    bool preRuleTimerEnabled = true;
    int preRuleTimerInterval = 0;

    // Initialize application state
    auto applicationSettingsState = initializeApplicationPersistence(preRulesEnabled,preRuleTimerEnabled,preCloseOnExit,preRuleTimerInterval);

    bool expectedCloseOnExit = true;
    bool exprectedRulesEnabled = false;
    bool expectedRuleTimerEnabled = true;
    int expectedRuleTimerInterval = 0;


    auto result = expectedCloseOnExit == applicationSettingsState->isCloseOnExitEnabled() &&
            exprectedRulesEnabled == applicationSettingsState->isRulesEnabled() &&
            expectedRuleTimerEnabled == applicationSettingsState->isRuleTimerEnabled() &&
             expectedRuleTimerInterval == applicationSettingsState->ruleTimerInterval();

    QVERIFY(result);
}

void Core_functionality::testApplicationPersistenceFail1()
{
    // Pre state variables

    bool preRulesEnabled = false;
    bool preCloseOnExit = true;
    bool preRuleTimerEnabled = true;
    int preRuleTimerInterval = 15;

    // Initialize application state
    auto applicationSettingsState = initializeApplicationPersistence(preRulesEnabled,preRuleTimerEnabled,preCloseOnExit,preRuleTimerInterval);

    bool expectedCloseOnExit = true;
    bool exprectedRulesEnabled = false;
    bool expectedRuleTimerEnabled = true;
    int expectedRuleTimerInterval = 0;


    auto result = expectedCloseOnExit == applicationSettingsState->isCloseOnExitEnabled() &&
            exprectedRulesEnabled == applicationSettingsState->isRulesEnabled() &&
            expectedRuleTimerEnabled == applicationSettingsState->isRuleTimerEnabled() &&
             expectedRuleTimerInterval == applicationSettingsState->ruleTimerInterval();

    QVERIFY(!result);
}

void Core_functionality::insert_rule_filename_match_success_1()
{
   auto rConfig = new DefaultRuleConfiguration();

    rConfig->setTitle("Test1");
    rConfig->setAppliesTo(TEST_WORKING_PATH);
    rConfig->setAction(RulesContext::DeleteAction);

    auto cConfig = new DefaultCriteriaConfiguration();

    cConfig->setKeywords(QStringList() << "T1" << "T2");
    cConfig->setCriteria(RulesContext::FileNameMode);
    cConfig->setCompareCriteria(RulesContext::Match);

    auto configs = QList<const IDefaultConditionConfigurator*>() << cConfig;

    auto result = testInsertRule(rConfig,configs);

    QVERIFY(result);
}

void Core_functionality::insert_rule_filename_match_succes_2()
{
    auto rConfig = new DefaultRuleConfiguration();

    rConfig->setTitle("Test2");
    rConfig->setAppliesTo(TEST_WORKING_PATH);
    rConfig->setAction(RulesContext::DeleteAction);

    auto cConfig = new DefaultCriteriaConfiguration();

    cConfig->setKeywords(QStringList() << "G1" << "G2");
    cConfig->setCriteria(RulesContext::FileSizeMode);
    cConfig->setCompareCriteria(RulesContext::Contain);

    auto configs = QList<const IDefaultConditionConfigurator*>() << cConfig;

    auto result = testInsertRule(rConfig,configs);

    QVERIFY(result);
}


void Core_functionality::insert_rule_created_before_succes1()
{
    auto rConfig = new DefaultRuleConfiguration();

    rConfig->setTitle("Test3");
    rConfig->setAppliesTo(TEST_WORKING_PATH);
    rConfig->setAction(RulesContext::DeleteAction);

    auto cConfig = new DefaultCriteriaConfiguration();

    cConfig->setKeywords(QStringList() << "G1" << "G2");
    cConfig->setCriteria(RulesContext::FileCreatedMode);
    cConfig->setCompareCriteria(RulesContext::OlderThan);
    cConfig->setDate(QDateTime(QDate(2012,6,3)));

    auto configs = QList<const IDefaultConditionConfigurator*>() << cConfig;

    auto result = testInsertRule(rConfig,configs);

    QVERIFY(result);
}

void Core_functionality::insert_rule_datecreated_after_succes1()
{
    auto rConfig = new DefaultRuleConfiguration();

    rConfig->setTitle("Test4");
    rConfig->setAppliesTo(TEST_WORKING_PATH);
    rConfig->setAction(RulesContext::DeleteAction);

    auto cConfig = new DefaultCriteriaConfiguration();

    cConfig->setKeywords(QStringList() << "G1" << "G2");
    cConfig->setCriteria(RulesContext::FileCreatedMode);
    cConfig->setCompareCriteria(RulesContext::YoungerThan);
    cConfig->setDate(QDateTime(QDate(2012,6,3)));

    auto configs = QList<const IDefaultConditionConfigurator*>() << cConfig;

    auto result = testInsertRule(rConfig,configs);

    QVERIFY(result);
}

void Core_functionality::insert_rule_sizeinterval_success_1()
{
    auto rConfig = new DefaultRuleConfiguration();

    rConfig->setTitle("Test5");
    rConfig->setAppliesTo(TEST_WORKING_PATH);
    rConfig->setAction(RulesContext::DeleteAction);

    auto cConfig = new DefaultCriteriaConfiguration();

    cConfig->setKeywords(QStringList() << "G1" << "G2");
    cConfig->setCriteria(RulesContext::FileSizeMode);
    cConfig->setCompareCriteria(RulesContext::Interval);

    auto lowUnits = 24;
    auto lowDSU = "mb";

    auto highUnits = 48;
    auto highDSU = "mb";

    cConfig->setSizeInterval(lowUnits,lowDSU,highUnits,highDSU);

    auto configs = QList<const IDefaultConditionConfigurator*>() << cConfig;

    auto result = testInsertRule(rConfig,configs);

    QVERIFY(result);
}


void Core_functionality::operationFilenameMatchSuccess1()
{
    /*
     * INITIAL STATE:
     *  - Create dummy files in folder 'test_folder'
     */

    auto fileCreator = new  TestFileCreator();

    auto ruleConfig = new DefaultRuleConfiguration();
    ruleConfig->setAction(RulesContext::DeleteAction);
    ruleConfig->setDeepScanMode(true);
    ruleConfig->setType(RulesContext::Action);
    ruleConfig->setAppliesTo(TEST_WORKING_PATH);

    auto criteriaConfig = new DefaultCriteriaConfiguration();

    criteriaConfig->setCriteria(RulesContext::FileBaseMode);
    criteriaConfig->setCompareCriteria(RulesContext::Match);

    auto preKeywords = QStringList() << "FCK" << "README";

    criteriaConfig->setKeywords(preKeywords);

    auto criteriaConfigs = QList<const IDefaultConditionConfigurator*>() << criteriaConfig;

    auto objects = initializePreState(ruleConfig,criteriaConfigs,fileCreator);

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
        for (auto preKeyword : preKeywords) {
            auto subject = info.baseName();

            if(subject == preKeyword)
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
        actualList = fileCreator->VirtualObjects(TEST_WORKING_PATH);
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
        cleaned_up = fileCreator->emptyTestFolder(TEST_WORKING_PATH);
    } catch (const char *msg) {
        printf("%s\n",msg);
        return;
    }


    if(!cleaned_up)
        printf(DELETE_STATUS);

    QVERIFY(referenceList == actualList);
}

void Core_functionality::operationFilenameContainSuccess1()
{
    /*
     * INITIAL STATE:
     *  - Create dummy files in folder 'test_folder'
     */

    auto fileCreator = new  TestFileCreator();

    auto ruleConfig = new DefaultRuleConfiguration();
    ruleConfig->setAction(RulesContext::DeleteAction);
    ruleConfig->setDeepScanMode(true);
    ruleConfig->setType(RulesContext::Action);
    ruleConfig->setAppliesTo(TEST_WORKING_PATH);

    auto criteriaConfig = new DefaultCriteriaConfiguration();

    criteriaConfig->setCriteria(RulesContext::FileBaseMode);
    criteriaConfig->setCompareCriteria(RulesContext::Contain);

    auto preKeywords = QStringList() << "FCK" << "README";

    criteriaConfig->setKeywords(preKeywords);

    auto criteriaConfigs = QList<const IDefaultConditionConfigurator*>() << criteriaConfig;

    auto objects = initializePreState(ruleConfig,criteriaConfigs,fileCreator);

    Virtual_Objects referenceList;

    for (int i = 0; i < objects->count(); ++i) {
        VIRTUAL_FILE_OBJECT obj = objects->getVirtualObjectFromIndex(i);
        QFileInfo info = obj.additionalInformation;
        bool contains = false;
        for (auto keyword : preKeywords) {
            auto subject = info.fileName();

            if(subject.contains(keyword))
               contains = true;
        }
        if(!contains)
            referenceList << obj;
    }

    mApp->clearFoldersAccordingToRules(mApp->watchFolders());

    /*
     * Note regarding asynchronious calls:
     *  Due to asynchonously calls to both FileWorkOperationWorker and EntityQueueManager tests have to wait a little amount of time
     *  to ensure proper sync between the caller and the called classes
     */

    QThread::sleep(SLEEP_SEC);

    Virtual_Objects actualList;
    try {
        actualList = fileCreator->VirtualObjects(TEST_WORKING_PATH);
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
        cleaned_up = fileCreator->emptyTestFolder(TEST_WORKING_PATH);
    } catch (const char *msg) {
        printf("%s\n",msg);
        return;
    }

    if(!cleaned_up)
        printf(DELETE_STATUS);

    QVERIFY(referenceList == actualList);
}

void Core_functionality::operation_extension_match_success_1()
{
    /*
     * INITIAL STATE:
     *  - Create dummy files in folder 'test_folder'
     */

    auto fileCreator = new  TestFileCreator();

    auto ruleConfig = new DefaultRuleConfiguration();
    ruleConfig->setAction(RulesContext::DeleteAction);
    ruleConfig->setDeepScanMode(true);
    ruleConfig->setType(RulesContext::Action);
    ruleConfig->setAppliesTo(TEST_WORKING_PATH);

    auto criteriaConfig = new DefaultCriteriaConfiguration();

    criteriaConfig->setCriteria(RulesContext::FileExtensionMode);
    criteriaConfig->setCompareCriteria(RulesContext::Contain);

    auto preKeywords = QStringList() << "txt" << "pdf";

    criteriaConfig->setKeywords(preKeywords);

    auto criteriaConfigs = QList<const IDefaultConditionConfigurator*>() << criteriaConfig;

    auto objects = initializePreState(ruleConfig,criteriaConfigs,fileCreator);

    Virtual_Objects referenceList;

    for (int i = 0; i < objects->count(); ++i) {
        VIRTUAL_FILE_OBJECT obj = objects->getVirtualObjectFromIndex(i);
        QFileInfo info = obj.additionalInformation;
        bool match = false;
        for (auto keyword : preKeywords) {
            auto subject = info.suffix();
            match = (subject == keyword) ?  true : match;
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
        actualList = fileCreator->VirtualObjects(TEST_WORKING_PATH);
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
        cleaned_up = fileCreator->emptyTestFolder(TEST_WORKING_PATH);
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

    auto fileCreator = new  TestFileCreator();

    auto ruleConfig = new DefaultRuleConfiguration();
    ruleConfig->setAction(RulesContext::DeleteAction);
    ruleConfig->setDeepScanMode(true);
    ruleConfig->setType(RulesContext::Action);

    auto criteriaConfig = new DefaultCriteriaConfiguration();

    criteriaConfig->setCriteria(RulesContext::FileSizeMode);
    criteriaConfig->setCompareCriteria(RulesContext::LesserThan);

    auto sizeUnits = 256;
    auto sizeDSU = "kb";

    criteriaConfig->setSizeLimit(SizeLimit(sizeUnits,sizeDSU));

    auto criteriaConfigs = QList<const IDefaultConditionConfigurator*>() << criteriaConfig;

    auto objects = initializePreState(ruleConfig,criteriaConfigs,fileCreator);

    Virtual_Objects referenceList;
    qint64 bytes = FCU::convertToBytes(sizeUnits,sizeDSU);

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
        actualList = fileCreator->VirtualObjects(TEST_WORKING_PATH);
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
        cleaned_up = fileCreator->emptyTestFolder(TEST_WORKING_PATH);
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

    auto fileCreator = new  TestFileCreator();

    auto ruleConfig = new DefaultRuleConfiguration();
    ruleConfig->setAction(RulesContext::DeleteAction);
    ruleConfig->setDeepScanMode(true);
    ruleConfig->setType(RulesContext::Action);

    auto criteriaConfig = new DefaultCriteriaConfiguration();

    criteriaConfig->setCriteria(RulesContext::FileSizeMode);
    criteriaConfig->setCompareCriteria(RulesContext::Equal);

    auto sizeUnits = 256;
    auto sizeDSU = "kb";

    criteriaConfig->setSizeLimit(SizeLimit(sizeUnits,sizeDSU));

    auto criteriaConfigs = QList<const IDefaultConditionConfigurator*>() << criteriaConfig;

    auto objects = initializePreState(ruleConfig,criteriaConfigs,fileCreator);

    Virtual_Objects referenceList;
    auto bytes = FCU::convertToBytes(sizeUnits,sizeDSU);

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
        actualList = fileCreator->VirtualObjects(TEST_WORKING_PATH);
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
        cleaned_up = fileCreator->emptyTestFolder(TEST_WORKING_PATH);
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

    auto fileCreator = new  TestFileCreator();

    auto ruleConfig = new DefaultRuleConfiguration();
    ruleConfig->setAction(RulesContext::DeleteAction);
    ruleConfig->setDeepScanMode(true);
    ruleConfig->setType(RulesContext::Action);

    auto criteriaConfig = new DefaultCriteriaConfiguration();

    criteriaConfig->setCriteria(RulesContext::FileSizeMode);
    criteriaConfig->setCompareCriteria(RulesContext::LesserOrEqualThan);

    auto sizeUnits = 256;
    auto sizeDSU = "kb";

    criteriaConfig->setSizeLimit(SizeLimit(sizeUnits,sizeDSU));

    auto criteriaConfigs = QList<const IDefaultConditionConfigurator*>() << criteriaConfig;

    auto objects = initializePreState(ruleConfig,criteriaConfigs,fileCreator);


    Virtual_Objects referenceList;
    qint64 bytes = FCU::convertToBytes(sizeUnits,sizeDSU);

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
        actualList = fileCreator->VirtualObjects(TEST_WORKING_PATH);
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
        cleaned_up = fileCreator->emptyTestFolder(TEST_WORKING_PATH);
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

    auto fileCreator = new  TestFileCreator();

    auto ruleConfig = new DefaultRuleConfiguration();
    ruleConfig->setAction(RulesContext::DeleteAction);
    ruleConfig->setDeepScanMode(true);
    ruleConfig->setType(RulesContext::Action);

    auto criteriaConfig = new DefaultCriteriaConfiguration();

    criteriaConfig->setCriteria(RulesContext::FileSizeMode);
    criteriaConfig->setCompareCriteria(RulesContext::GreaterOrEqualThan);

    auto sizeUnits = 256;
    auto sizeDSU = "kb";

    criteriaConfig->setSizeLimit(SizeLimit(sizeUnits,sizeDSU));

    auto criteriaConfigs = QList<const IDefaultConditionConfigurator*>() << criteriaConfig;

    auto objects = initializePreState(ruleConfig,criteriaConfigs,fileCreator);

    Virtual_Objects referenceList;
    auto bytes = FCU::convertToBytes(sizeUnits,sizeDSU);

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
        actualList = fileCreator->VirtualObjects(TEST_WORKING_PATH);
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
        cleaned_up = fileCreator->emptyTestFolder(TEST_WORKING_PATH);
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

    auto fileCreator = new  TestFileCreator();

    auto ruleConfig = new DefaultRuleConfiguration();
    ruleConfig->setAction(RulesContext::DeleteAction);
    ruleConfig->setDeepScanMode(true);
    ruleConfig->setType(RulesContext::Action);

    auto criteriaConfig = new DefaultCriteriaConfiguration();

    criteriaConfig->setCriteria(RulesContext::FileSizeMode);
    criteriaConfig->setCompareCriteria(RulesContext::GreaterThan);

    auto sizeUnits = 256;
    auto sizeDSU = "kb";

    criteriaConfig->setSizeLimit(SizeLimit(sizeUnits,sizeDSU));

    auto criteriaConfigs = QList<const IDefaultConditionConfigurator*>() << criteriaConfig;

    auto objects = initializePreState(ruleConfig,criteriaConfigs,fileCreator);

    Virtual_Objects referenceList;
    qint64 bytes = FCU::convertToBytes(sizeUnits,sizeDSU);

    for (int i = 0; i < objects->count(); ++i) {
        VIRTUAL_FILE_OBJECT obj;
        try {
            obj = objects->getVirtualObjectFromIndex(i);
        }  catch (std::out_of_range *e) {
            cout << e->what() << endl;
            return Q_ASSERT(false);
        }
        qint64 fileSize = obj.additionalInformation.size();
        if(fileSize > bytes)
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
        actualList = fileCreator->VirtualObjects(TEST_WORKING_PATH);
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
        cleaned_up = fileCreator->emptyTestFolder(TEST_WORKING_PATH);
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

    auto fileCreator = new  TestFileCreator();

    auto ruleConfig = new DefaultRuleConfiguration();
    ruleConfig->setAction(RulesContext::DeleteAction);
    ruleConfig->setDeepScanMode(true);
    ruleConfig->setType(RulesContext::Action);

    auto criteriaConfig = new DefaultCriteriaConfiguration();

    criteriaConfig->setCriteria(RulesContext::FileSizeMode);
    criteriaConfig->setCompareCriteria(RulesContext::Interval);

    auto lowerSizeUnits = 192;
    auto lowerSizeDSU = "kb";

    auto upperSizeUnits = 512;
    auto upperSizeDSU = "kb";

    criteriaConfig->setSizeInterval(lowerSizeUnits,lowerSizeDSU,upperSizeUnits,upperSizeDSU);

    auto criteriaConfigs = QList<const IDefaultConditionConfigurator*>() << criteriaConfig;

    auto objects = initializePreState(ruleConfig,criteriaConfigs,fileCreator);

    Virtual_Objects referenceList;

    for (int i = 0; i < objects->count(); ++i) {
        auto object = objects->getVirtualObjectFromIndex(i);
        auto subject = object.additionalInformation.size();
        if(subject < FCU::convertToBytes(lowerSizeUnits,lowerSizeDSU) || subject > FCU::convertToBytes(upperSizeUnits,upperSizeDSU))
            referenceList << object;
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
        actualList = fileCreator->VirtualObjects(TEST_WORKING_PATH);
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
        cleaned_up = fileCreator->emptyTestFolder(TEST_WORKING_PATH);
    } catch (const char *msg) {
        printf("%s\n",msg);
        return;
    }

    if(!cleaned_up)
        printf(DELETE_STATUS);

    QVERIFY(referenceList == actualList);
}

void Core_functionality::operation_move_filename_match_success_1()
{

}

const Virtual_Objects *Core_functionality::initializePreState(const IDefaultRuleConfigurator *ruleConfig,
                                                              QList<const IDefaultConditionConfigurator *> ruleConditionConfigs,
                                                              TestFileCreator *fileCreator)
{
    // Pre-state variables

    auto ruleBuilder = new RuleBuilder();

    mApp->addWatchFolder(TEST_WORKING_PATH);

    QList<const DefaultRuleCriteria*> criterias;

    for (auto config : ruleConditionConfigs)
        criterias << ruleBuilder->buildCriteria(config);

    auto preRule = ruleBuilder->buildRule(ruleConfig,criterias);

    mApp->insertRule(preRule);

    const Virtual_Objects *objects;

    try {
        objects = fileCreator->createFiles(TEST_WORKING_PATH,test_file_set_1);
    } catch (const char *msg) {
        printf("%s\n",msg);
        throw "Creating files failed.";
    }

    return objects;
}


    QTEST_MAIN(Core_functionality)

#include "tst_core_functionality.moc"
#endif
