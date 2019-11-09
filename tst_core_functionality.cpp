#include <QtTest>
#include <QCoreApplication>

#include "mainapplication.h"

class Core_functionality : public QObject
{
    Q_OBJECT

public:
    Core_functionality();
    ~Core_functionality();

private slots:
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

#ifdef TEST_MODE
    QTEST_MAIN(Core_functionality)
#endif

#include "tst_core_functionality.moc"
