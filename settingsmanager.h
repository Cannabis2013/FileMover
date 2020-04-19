#ifndef SETTINGSCONTROLLER_H
#define SETTINGSCONTROLLER_H

#include <QTimer>
#include <qstring.h>
#include <QTreeWidgetItem>
#include "qlist.h"
#include "abstractpersistence.h"
#include "icon.h"
#include <qdir.h>
#include <qdiriterator.h>
#include "isettingsmodel.h"
#include "entitymodel.h"
#include "imutableobject.h"
#include <qdatastream.h>
#include "abstractsettingsmanager.h"
#include "isettingsbuilder.h"
#include "ientitymodelbuilder.h"


class settingsManager : public AbstractSettingsManager,
        private AbstractPersistence
{
    Q_OBJECT
public:
    settingsManager(const QString &appName,
                    const QString &orgName,
                    ISettingsBuilder<QRect> *builderService,
                    IEntityModelBuilder<DefaultModelInterface,DefaultFileModelList> *modelBuilderService);

    ~settingsManager();

    // Persistence
    void readSettings() override;
    void writeSettings() override;

    // Basic settings
    void setCloseOnExit(bool enable) override;
    void setRulesEnabled(bool enable) override;
    void setTimerEnabled(bool enable) override;
    void setTimerInterval(int msec) override;

    // Icons
    void insertIcon(const AbstractIcon *ic) override {trayIconList << ic;}
    void insertIcons(const QList<const AbstractIcon*> &icons) override {trayIconList << icons;}

    const ISettingsModel<QRect>* settingsState() const override;
    void setSettings(const ISettingsModel<QRect> *s) override;

    // Path Related..

    int folderCount() const override {return watchFolders.count();}

    void insertPath(QString path) override;
    void insertPath(const QStringList& paths) override;

    void removePath(QString path) override;
    void removePathAt(int index) override;
    void clearPaths() override;

    // Request file object processing

    void requestProcess();

    QStringList paths() const override {return watchFolders;}
    QList<QTreeWidgetItem *> pathItems() const override;

    bool closeOnQuit() const override {return _settings->isCloseOnExitEnabled();}
    bool isRulesEnabled() const override {return _settings->isRulesEnabled();}
    bool countTimerEnabled() const override {return _settings->isRuleTimerEnabled();}
    QString countTimerInterval() const override {return QString::number(_settings->ruleTimerInterval());}

    // Icons related..
    QList<const AbstractIcon*> allIcons() const override {return trayIconList;}

private:
    QList<const AbstractIcon *> scanForIcons(QString path);

    QStringList watchFolders;
    QString ressourceFolder = "Ressources";
    QString fileIconPath = "fileIcons";
    ISettingsBuilder<QRect> *const _settingsBuilder;
    const ISettingsModel<QRect>* _settings;
    QTimer countTimer;
    QList<const AbstractIcon*> trayIconList;
    QList<const AbstractIcon*> fileIconList;
    const AbstractIcon *currentTrayIcon;
    const AbstractIcon * fileIconStandard;
    IEntityModelBuilder<DefaultModelInterface,DefaultFileModelList> *const _modelBuilderService;
};

typedef settingsManager sM;

#endif // SETTINGSCONTROLLER_H
