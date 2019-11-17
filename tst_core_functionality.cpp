#include <QtTest>
#include <QCoreApplication>

#include "mainapplication.h"

#ifdef TEST_MODE
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

private:
    MainApplication *mApp;
};

Core_functionality::Core_functionality()
{
    mApp = new MainApplication("MHTest","MH");
}

Core_functionality::~Core_functionality()
{

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

    QString preAPath = "/testpath", preTitle = "Test";
    QStringList prekWrds = QStringList() << "T1" << "T2";
    rD::fileConditionEntity preCond = rD::filepathMode;
    rD::fileCompareEntity preComp = rD::match;
    Rule r;

    // Initialize pre-state

    r.title = preTitle;
    SubRule sR;
    r.appliesToPath = preAPath;
    sR.keyWords = prekWrds;
    sR.fieldCondition = preCond;
    sR.fileCompareMode = preComp;

    r.subRules << sR;

    mApp->insertRule(r);

    // Post section


    Rule tR = mApp->rule(preTitle);

    QVERIFY(r == tR);
}

void Core_functionality::insert_rule_filepath_match_fail_1()
{
    // Pre-state variables

    QString preAPath = "/testpath", preTitle = "Test";
    QStringList prekWrds = QStringList() << "T1" << "T2";
    rD::fileConditionEntity preCond = rD::filepathMode;
    rD::fileCompareEntity preComp = rD::match;
    Rule r;

    // Initialize pre-state
    SubRule sR;

    r.title = preTitle;
    r.appliesToPath = preAPath;
    sR.keyWords = prekWrds;
    sR.fieldCondition = preCond;
    sR.fileCompareMode = preComp;

    r.subRules << sR;

    mApp->insertRule(r);

    // Post section

    QString postAPath = "/testpath", postTitle = "NotTest";
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

    Rule tR = mApp->rule(preTitle);

    QVERIFY(!(tR == compareRule));
}

void Core_functionality::insert_rule_filepath_match_fail_2()
{
    // Pre-state variables

    QString preAPath = "/testpath", preTitle = "Test";
    QStringList prekWrds = QStringList() << "T1" << "T2";
    rD::fileConditionEntity preCond = rD::filepathMode;
    rD::fileCompareEntity preComp = rD::match;
    Rule r;

    // Initialize pre-state

    r.title = preTitle;
    SubRule sR;
    r.appliesToPath = preAPath;
    sR.keyWords = prekWrds;
    sR.fieldCondition = preCond;
    sR.fileCompareMode = preComp;

    r.subRules << sR;

    mApp->insertRule(r);

    // Post section

    QString postAPath = "/testpath", postTitle = "Test";
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

    Rule tR = mApp->rule(preTitle);

    QVERIFY(!(tR == compareRule));
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
    sR.fixedDate = QPair<rD::fileCompareEntity,myDateTime>(rD::olderThan,mDate);

    preRule.subRules << sR;

    mApp->insertRule(preRule);

    Rule postRule = mApp->rule(title);

    QVERIFY(preRule == postRule);
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
    sR.fixedDate = QPair<rD::fileCompareEntity,myDateTime>(rD::youngerThan,mDate);

    preRule.subRules << sR;

    mApp->insertRule(preRule);

    Rule postRule = mApp->rule(title);

    QVERIFY(preRule == postRule);
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
    sR1.fixedDate = QPair<rD::fileCompareEntity,myDateTime>(rD::olderThan,originalDate);

    preRule.subRules << sR1;

    sR2.fieldCondition = rD::dateCreatedMode;
    sR2.fileCompareMode = rD::olderThan;
    sR2.fixedDate = QPair<rD::fileCompareEntity,myDateTime>(rD::olderThan,compareDate);

    compareRule.subRules << sR2;


    mApp->insertRule(preRule);

    Rule postRule = mApp->rule(title);

    QVERIFY(!(compareRule == postRule));
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
    sR1.fixedDate = QPair<rD::fileCompareEntity,myDateTime>(rD::olderThan,originalDate);

    preRule.subRules << sR1;

    sR2.fieldCondition = rD::dateCreatedMode;
    sR2.fileCompareMode = rD::youngerThan;
    sR2.fixedDate = QPair<rD::fileCompareEntity,myDateTime>(rD::olderThan,originalDate);

    compareRule.subRules << sR2;


    mApp->insertRule(preRule);

    Rule postRule = mApp->rule(title);

    QVERIFY(!(compareRule == postRule));
}


    QTEST_MAIN(Core_functionality)

#include "tst_core_functionality.moc"
#endif

