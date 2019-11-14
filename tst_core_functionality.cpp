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

    // Rule related
    void insert_rule_success_1();
    void insert_rule_fail_1();
    void insert_rule_fail_2();


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



void Core_functionality::insert_rule_success_1()
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

    r.subRules = QList<SubRule>() << sR;

    mApp->insertRule(r);

    // Post section

    // Expected post state variables

    QString postAPath = "/testpath", postTitle = "Test";
    QStringList postkWrds = QStringList() << "T1" << "T2";
    rD::fileConditionEntity postCond = rD::filepathMode;
    rD::fileCompareEntity postComp = rD::match;

    Rule tR = mApp->rule(preTitle);

    bool test_state = true;

    if(tR.title != postTitle || tR.appliesToPath != postAPath)
        test_state = false;

    SubRule tSR = tR.subRules.first();
    if(tSR.fieldCondition != postCond || tSR.fileCompareMode != postComp)
        test_state = false;
    if(tSR.keyWords != postkWrds)
        test_state = false;

    QVERIFY(test_state);
}

void Core_functionality::insert_rule_fail_1()
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

    r.subRules = QList<SubRule>() << sR;

    mApp->insertRule(r);

    // Post section

    // Expected post state variables

    QString postAPath = "/testpath", postTitle = "NotTest";
    QStringList postkWrds = QStringList() << "T1" << "T2";
    rD::fileConditionEntity postCond = rD::filepathMode;
    rD::fileCompareEntity postComp = rD::match;

    Rule tR = mApp->rule(preTitle);

    bool test_state = true;

    if(tR.title != postTitle || tR.appliesToPath != postAPath)
        test_state = false;

    SubRule tSR = tR.subRules.first();
    if(tSR.fieldCondition != postCond || tSR.fileCompareMode != postComp)
        test_state = false;
    if(tSR.keyWords != postkWrds)
        test_state = false;

    QVERIFY(!test_state);
}

void Core_functionality::insert_rule_fail_2()
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

    r.subRules = QList<SubRule>() << sR;

    mApp->insertRule(r);

    // Post section

    // Expected post state variables

    QString postAPath = "/testpath", postTitle = "Test";
    QStringList postkWrds = QStringList() << "T1" << "T2";
    rD::fileConditionEntity postCond = rD::extensionMode;
    rD::fileCompareEntity postComp = rD::match;

    Rule tR = mApp->rule(preTitle);

    bool test_state = true;

    if(tR.title != postTitle || tR.appliesToPath != postAPath)
        test_state = false;

    SubRule tSR = tR.subRules.first();
    if(tSR.fieldCondition != postCond || tSR.fileCompareMode != postComp)
        test_state = false;
    if(tSR.keyWords != postkWrds)
        test_state = false;

    QVERIFY(!test_state);
}


    QTEST_MAIN(Core_functionality)

#include "tst_core_functionality.moc"
#endif

