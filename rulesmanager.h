#ifndef RULECONTROLLER_H
#define RULECONTROLLER_H

#include <QListWidgetItem>
#include <qapplication.h>

#include "abstractpersistence.h"
#include "abstractrulesmanager.h"
#include "ifilelistservice.h"
#include "defaultRuleConfiguration.h"
#include "defaultrulescontext.h"

typedef IFileListService<IModelBuilder<IFileModel<QFileInfo>,QString>> ListService;

class rulesManager :
        public AbstractRulesManager,
        private AbstractPersistence
{
public:
    rulesManager(const QString &appName, const QString &orgName, IDefaultRuleBuilder *ruleBuilderService);
    ~rulesManager();

    DefaultFileModelList filterAccordingToCriterias(const DefaultFileModelList &list,
                                                    const IRule<IDefaultRuleCondition> *rule,
                                                    ListService *listService) override;

    // Persistence
    void readSettings() override;
    void writeSettings() override;

    // State manipulation
    void swapRule(int i, int j) override;
    void addRule(const IRule<IDefaultRuleCondition>* r) override;
    void addRules(const QList<const IRule<IDefaultRuleCondition>*> &r) override;
    void replaceRule(const IRule<IDefaultRuleCondition>* r, int index) override;
    void replaceRule(const IRule<IDefaultRuleCondition>* r, QString title) override;
    void removeRuleAt(const int &i) override {_rules.removeAt(i);}
    void removeRule(const QString &title) override;
    // Get
    QList<const IRule<IDefaultRuleCondition> *> rules() const override;
    const IRule<IDefaultRuleCondition> *rule(int index)  const override {return _rules.value(index);}
    const IRule<IDefaultRuleCondition> *rule(const QString &title) const override;
    int rulesCount() const override {return _rules.count();}

    // Get generic models
    QList<QTreeWidgetItem*>ruleItems() const override;

private:
    QList<const IRule<IDefaultRuleCondition>*> _rules;
};

#endif // RULECONTROLLER_H
