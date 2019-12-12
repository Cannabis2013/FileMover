#include <QtTest>
#include <QCoreApplication>

#include "mainapplication.h"
#include "testfilecreator.h"

#ifdef TEST_MODE

/*
  * TODO: Implement test functionality that tests the fileoperations done by FileWorker
 */


class HelperFunctions
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
                compOne.destinationPath != compTwo.destinationPath ||
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

const QString workingPath = "d:/Test_folder/";
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

    // Settings and persistence related
    void persistence_SettingsManager_Success_1();
    void persistence_SettingsManager_Success_2();

    /*
     * Rules section
     *  - rD::fileConditionEntity = filepathMode rD::fileCompareEntity = match
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

    void operation_file_set_one_delete_success();

private:
    MainApplication *mApp;
};

Core_functionality::Core_functionality()
{
    // Setup the core module
    mApp = new MainApplication("MHTest","MH");
}

Core_functionality::~Core_functionality()
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
    QVERIFY(HelperFunctions::RuleEquals(preRule,postRule));
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

    QVERIFY(!HelperFunctions::RuleEquals(compareRule,postRule));
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
    rD::fileConditionEntity postCond = rD::extensionMode;
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

    QVERIFY(!HelperFunctions::RuleEquals(compareRule,postRule));
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

    sR.fieldCondition = rD::dateCreatedMode;
    sR.fileCompareMode = rD::olderThan;
    sR.date = mDate;

    preRule.subRules << sR;

    mApp->insertRule(preRule);

    Rule postRule = mApp->rule(title);

    QVERIFY(HelperFunctions::RuleEquals(preRule,postRule));
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

    sR.fieldCondition = rD::dateCreatedMode;
    sR.fileCompareMode = rD::youngerThan;
    sR.date = mDate;

    preRule.subRules << sR;

    mApp->insertRule(preRule);

    Rule postRule = mApp->rule(title);

    QVERIFY(HelperFunctions::RuleEquals(preRule,postRule));
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

    sR1.fieldCondition = rD::dateCreatedMode;
    sR1.fileCompareMode = rD::olderThan;
    sR1.date = originalDate;

    preRule.subRules << sR1;

    sR2.fieldCondition = rD::dateCreatedMode;
    sR2.fileCompareMode = rD::olderThan;
    sR2.date = compareDate;

    compareRule.subRules << sR2;


    mApp->insertRule(preRule);

    Rule postRule = mApp->rule(title);

    QVERIFY(!HelperFunctions::RuleEquals(compareRule,postRule));
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

    sR1.fieldCondition = rD::dateCreatedMode;
    sR1.fileCompareMode = rD::olderThan;
    sR1.date = originalDate;

    preRule.subRules << sR1;

    sR2.fieldCondition = rD::dateCreatedMode;
    sR2.fileCompareMode = rD::youngerThan;
    sR2.date = originalDate;

    compareRule.subRules << sR2;


    mApp->insertRule(preRule);

    Rule postRule = mApp->rule(title);

    QVERIFY(!HelperFunctions::RuleEquals(compareRule,postRule));
}

void Core_functionality::insert_rule_sizeinterval_success_1()
{
    // Pre state variables
    QString title = "Size interval rule 1";
    Rule preRule;
    preRule.title = title;
    preRule.actionRuleEntity = rD::Delete;

    SubRule sR;

    sR.fieldCondition = rD::sizeMode;
    sR.fileCompareMode = rD::interval;

    SizeOperand minSize(244,"kb"),maxSize(512,"kb");
    sR.sizeInterval = SizeInterval(minSize,maxSize);

    preRule.subRules << sR;

    mApp->insertRule(preRule);

    Rule postRule = mApp->rule(title);

    QVERIFY(HelperFunctions::RuleEquals(preRule,postRule));
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

    sR1.fieldCondition = rD::sizeMode;
    sR1.fileCompareMode = rD::interval;

    sR2.fieldCondition = rD::sizeMode;
    sR2.fileCompareMode = rD::interval;


    SizeOperand minSize1(244,"kb"),maxSize1(512,"kb");
    SizeOperand minSize2(384,"kb"),maxSize2(768,"kb");
    sR1.sizeInterval = SizeInterval(minSize1,maxSize1);
    sR2.sizeInterval = SizeInterval(minSize2,maxSize2);

    preRule.subRules << sR1;
    compareRule.subRules << sR2;

    mApp->insertRule(preRule);

    Rule postRule = mApp->rule(title);

    QVERIFY(!HelperFunctions::RuleEquals(compareRule,postRule));
}

void Core_functionality::operation_file_set_one_delete_success()
{
    /*
     * Create dummy files in folder 'test_folder'
     */


    /*
     * TODO: Implement remove files section
     * TODO: Implement check to ensure the files intented to be removed has actually been removed. And only them.
     */

    TestFileCreator *f_creator;
    try {
        f_creator = new  TestFileCreator(workingPath,test_file_set_1 ,true);
    } catch (char *msg) {
        printf("%s\n",msg);
        QVERIFY(false);
        return;
    }

    mApp->addWatchFolder(workingPath);

    // Pre-state variables

    QString preAPath = workingPath, preTitle = "Test1";
    QStringList prekWrds = QStringList() << "Notes" << "FCK";
    rD::fileActionEntity preAction = rD::Delete;
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


    /*
     * Clear test folder
     */

    try {
        f_creator->emptyTestFolder();
    } catch (char *msg) {
        printf("%s\n",msg);
        QVERIFY(false);
        return;
    }




}

    QTEST_MAIN(Core_functionality)

#include "tst_core_functionality.moc"
#endif

