#include "applicationui.h"
#include "ui_mainwindow.h"

ApplicationUI::ApplicationUI(AbstractApplicationService *coreApplication,
                             IDefaultRuleBuilder *ruleBuilderService,
                             IRuleDefinitions *ruleDefinitionsService) :
    QMainWindow(),
    ui(new Ui::ApplicationUI)
{
    ui->setupUi(this);

    this->coreApplication = coreApplication;

    // Init..
    clBut = ui->clearButt;
    clearStatusTextTimer = new QTimer();
    clearTrayMenu = new QMenu;
    columnWidths = {384,32};
    countTimer = new QTimer;
    countTimerStatus = false;
    countTimerInterval = 1000*60;
    countTrayMenu = new QMenu;
    currentDir = QDir::currentPath();
    detailedFolderView = ui->detailView;
    detailedFolderViewMenu = new QMenu();
    detailedListFontSize = 8;
    fileInfoBrowser = ui->fileInformationBrowserView;
    fileStandard = QIcon();
    folderTrayMenu = new QMenu;
    laptopScreenSize = myScreenDimension(0,0,1280,800);
    watchFolderView = ui->WatchFolderView;
    normalListFontSize = 16;
    screenSize = myScreenDimension(QGuiApplication::screens().at(0)->geometry());
    statusLine = ui->statusLineEdit;
    suffixTree = ui->suffixTree;
    suffixHeader = suffixTree->header();
    tempKey = 0;
    tray = new QSystemTrayIcon();
    trayMenu = new QMenu;
    title = "FileFolderManager";
    viewFont = QFont("Times",12);
    watchFolderViewMenu = new QMenu;
    //Setting up path or command to open folders in OS..
#ifdef Q_OS_WIN32
    ePath = "c:\\Windows\\Explorer.exe ";
#elif defined(Q_OS_LINUX)
    ePath = "Linux? Get out of town!";
#elif defined(Q_OS_MAC)
    ePath = "open ";
#else
    ePath = "Is it your own operation system?";
#endif

    // Spacer Related..

#ifdef Q_OS_MAC
    ui->verticalSpacer->changeSize(0,40,QSizePolicy::Ignored,QSizePolicy::Fixed);
#endif

    // Inject services
    setRuleBuilderService(ruleBuilderService);
    setRulesDefinitionsService(ruleDefinitionsService);
    // Persistent Settings Related..

    readSettings();

    // mainFolderView related..
    watchFolderView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    watchFolderView->setStyleSheet("QTreeWidget::branch{display:none;}");
    setWindowTitle(title);
    watchFolderView->setIconSize(QSize(32,32));

    // mainFolderView Setting columnwidth..
    watchFolderView->setColumnCount(1);
    for(int w = 0;w < watchFolderView->topLevelItemCount();w++)
    {
        int width = columnWidths.at(w);
        watchFolderView->setColumnWidth(w,width);
    }
    watchFolderView->setHeaderHidden(true);
    watchFolderView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    watchFolderView->setContextMenuPolicy(Qt::CustomContextMenu);
    watchFolderView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    watchFolderView->setFont(createFont(fontType::listFont));
    watchFolderView->setStyleSheet("border:0px;padding:0;margin:0px;");

    // mainFolderView Menu..

    watchFolderViewMenu->setStyleSheet("icon-size:12px;");
    watchFolderViewMenu->setTitle("MyMenu");
    watchFolderViewMenu->addAction(fileStandard,tr("Åbn mappen i explorer"),
                        this,SLOT(explorerFolder(bool)));

    // detailedFolderview related..

    QStringList headers = ByteCollection::createHeader();
    detailedFolderView->setColumnCount(headers.count());
    detailedFolderView->setHeaderLabels(headers);

    detailedFolderView->setFont(createFont(fontType::detailedList));
    detailedFolderView->setStyleSheet("padding:0;margin:0px;");

    // detailedFolderMenu related..
    detailedFolderView->setContextMenuPolicy(Qt::CustomContextMenu);
    detailedFolderViewMenu->setTitle("MyMenu");
    detailedFolderViewMenu->addAction("Åbn mappen i explorer",this,SLOT(openFolder(bool)));

    // Buttons Related..

    // clearStatusTextTimer related..

    clearStatusTextTimer->setSingleShot(true);
    clearStatusTextTimer->setInterval(3000);

    // countTimer related..
    countTimer->setSingleShot(false);
    countTimer->setInterval(countTimerInterval);
    countTimerStatus ? countTimer->start() : countTimer->stop();
    // Eventfilter..
    tray->setIcon(QIcon(":/My Images/Ressources/cplusplus.jpg"));

    installEventFilter(this);

    // Systemtray & Iconselecter Related..

    countTrayMenu->setTitle("Tæl mappe:");
    countTrayMenu->setObjectName("countTrayMenu");
    clearTrayMenu->setTitle("Ryd mappe:");
    folderTrayMenu->setTitle("Mine mapper:");

    updateSubTrayMenus();

    trayMenu->addMenu(countTrayMenu);
    trayMenu->addMenu(clearTrayMenu);
    trayMenu->addMenu(folderTrayMenu);
    trayMenu->addAction(tr("Quit"));
    tray->setContextMenu(trayMenu);

    // Fileworker Related..

    qRegisterMetaType<QList<DirectoryEntityModel>>("QList<DirectoryCountEntity>");

    /*
     * Setup connections
     */

    // suffixTree..

    connect(suffixHeader,&QHeaderView::sectionDoubleClicked,this,&ApplicationUI::sortSuffixTreeColumn);

    // Systemtray..
    connect(tray,SIGNAL(messageClicked()),this,SLOT(trayMsgClicked()));
    connect(tray,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,SLOT(trayClicked(QSystemTrayIcon::ActivationReason)));
    connect(countTrayMenu,SIGNAL(triggered(QAction*)),
            SLOT(countMenuTriggered(QAction*)));
    connect(trayMenu,SIGNAL(triggered(QAction*)),
            this,SLOT(tMenuClicked(QAction*)));
    connect(folderTrayMenu,SIGNAL(triggered(QAction*)),
            this,SLOT(explorerMenuTriggered(QAction*)));
    connect(coreApplication,&AbstractApplicationService::sendSystemTrayMessage,this,&ApplicationUI::showSystemMessage);

    connect(coreApplication,&AbstractApplicationService::sendFolderSize,this,&ApplicationUI::folderContentRecieved);
    connect(coreApplication,&AbstractApplicationService::sendStatusMessage,this,&ApplicationUI::setStatusText);

    connect(clearStatusTextTimer,&QTimer::timeout,this,&ApplicationUI::clearStatusLine);

    connect(coreApplication,&AbstractApplicationService::stateChanged,this,&ApplicationUI::updateView);

    tray->show();

    updateView();
}


ApplicationUI::~ApplicationUI()
{
    delete ui;
}

void ApplicationUI::closeEvent(QCloseEvent *cE)
{
    if(coreApplication->closeOnExit()) {
        cE->accept();
        writeSettings();
        delete coreApplication;
        tray->hide();
    }
    else {
        cE->ignore();
        hide();
        tray->show();
    }
}

void ApplicationUI::keyPressEvent(QKeyEvent *kE)
{
    tempKey = kE->key();
}

void ApplicationUI::keyReleaseEvent(QKeyEvent *kR)
{
    Q_UNUSED(kR)
    tempKey = 0;
}

void ApplicationUI::mousePressEvent(QMouseEvent *mp)
{
    offset = mp->pos();
}

void ApplicationUI::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)
    {
        move(mapToParent(e->pos() - offset));
    }
}

void ApplicationUI::explorerMenuTriggered(QAction *xAction)
{
    QStringList arg;
    if(xAction->text() != "Alle mapper")
    {

        QString txt = xAction->text(),
        #ifdef __MINGW64__
                ex = ePath + modifyPath(txt,"\\");
        #elif Q_OS_MAC
                ex = ePath + getItemFromList(txt);
        #elif __GNUC__
                ex = "";
                throw "Functionality is limited in linux";
        #else
                return;
        #endif
        char *command = ex.toLocal8Bit().data();
        system(command);
    }
    else
    {
        QStringList list = coreApplication->watchFolders();
        for(QString l : list)
        {
            QString ex = ePath;
#ifdef Q_OS_WIN32
            ex += modifyPath(l,"\\");
#elif defined Q_OS_MAC
            ex += l;
#else
            return;
#endif
            char *command = ex.toLocal8Bit().data();
            system(command);
        }
    }
}

void ApplicationUI::trayMsgClicked()
{
    show();
}

void ApplicationUI::msgToTray(const QString &msg)
{
    tray->showMessage("Error",msg,QSystemTrayIcon::Warning);
}

void ApplicationUI::iconSelected(QIcon ico)
{
    tray->setIcon(ico);
}

void ApplicationUI::trayClicked(QSystemTrayIcon::ActivationReason res)
{
    if(res == QSystemTrayIcon::DoubleClick)
    {
        if(isHidden())
            show();
        else
            hide();
    }
    if(res == QSystemTrayIcon::Context)
    {
        QRect geo = tray->geometry();
        QPoint tPos;
        tPos.setX(geo.x());
        tPos.setY(geo.y());
        trayMenu->show();
    }
}

void ApplicationUI::tMenuClicked(QAction *a)
{
    QString txt = a->text();
    if(txt == "Quit")
        close();
}

void ApplicationUI::countMenuTriggered(QAction *cAction)
{
    QString fPath = cAction->text();
    coreApplication->calculateFolderSize(fPath);
}

void ApplicationUI::clearMenuTriggered(QAction *clAction)
{
    if(clAction->text() == "Alle")
    {

    }
    else
    {

    }
}

void ApplicationUI::explorerFolder(bool ok)
{
    Q_UNUSED(ok)
    int r = watchFolderView->currentIndex().row();
    QStringList list = coreApplication->watchFolders();
#ifdef Q_OS_MAC

    QString path = ePath + list.at(r);
    char *command = path.toLocal8Bit().data();
    system(command);

#elif defined Q_OS_WIN32
    QString path = ePath + modifyPath(list.at(r),"\\");
    unique_ptr<QProcess>process(new QProcess);
    process->start(path);
    process->waitForFinished();
#elif defined Q_OS_LINUX
    cout << "Linux not supportet yet!" << endl;
#else
    cout << "Unknown operation system not supportet!" << endl;
#endif
}

void ApplicationUI::actionCountFolder(bool f)
{
    Q_UNUSED(f)
    if(watchFolderView->topLevelItemCount() == 0)
        return;
    QStringList list {currentMainFolderPath()};
    emit StartCount(list);
}

void ApplicationUI::setTimerStatus(bool makeActive)
{
    makeActive ? countTimer->start() : countTimer->stop();
}


void ApplicationUI::detailedFolderMenuCalled(QPoint p)
{
    if(!detailedFolderView->currentIndex().isValid())
        return;

    QPoint P = detailedFolderView->mapToGlobal(p);
    int Y = P.y();
    P.setY(Y + 12);

    detailedFolderViewMenu->popup(P);
}

void ApplicationUI::openFolder(bool ok)
{
    Q_UNUSED(ok)

    auto g = [](QString a)->QString
    {
        int index = a.count() -1;
        bool condition = true;
        while(condition)
        {
            if(a.at(index) == "/")
                return a.chopped((a.count() -1) - index);
            index--;
        }
        return a;
    };
    QTreeWidgetItem *item = detailedFolderView->currentItem();
    if(item == nullptr)
        return;

    QString path = modifyPath(g(item->text(1)),"\\"),
            toCommand = "explorer.exe \\select," + path;
    char *command = toCommand.toLocal8Bit().data();
    system(command);
}

void ApplicationUI::clearCompleted(bool a)
{
    QString txt,msg;
    if(a)
        txt = "Succeded";
    else
        txt = "Failed";

    msg = "Folder removal " + txt;
    tray->showMessage("Removal",msg);
}

void ApplicationUI::folderContentRecieved(const DirectoryEntityModel *entity)
{
    QString sizeNotation;
    double scaledAndRoundedSize = FCU::convertFromBytes(entity->size(),sizeNotation,2);
    QString folderName = entity->path(),
            folderSize = QString::number(scaledAndRoundedSize),
            message = QString("Size of folder content is %1 %2").arg(folderSize).arg(sizeNotation);
    showSystemMessage(folderName,message);
    delete entity;
}

void ApplicationUI::contextMenuCalled(QPoint p)
{
    if(!watchFolderView->currentIndex().isValid())
        return;

    QPoint P = watchFolderView->mapToGlobal(p);
    int Y = P.y();
    P.setY(Y + 12);

    watchFolderViewMenu->popup(P);
}

void ApplicationUI::updateDetaileditems()
{
    detailedFolderView->clear();
    auto itemList = coreApplication->detailedWatchFolderItems();
    detailedFolderView->addTopLevelItems(itemList);
}

void ApplicationUI::updateWatchFolderView()
{
    watchFolderView->clear();
    QList<QTreeWidgetItem*> watchFolders = coreApplication->watchFolderItems();
    watchFolderView->addTopLevelItems(watchFolders);
}

QString ApplicationUI::modifyPath(QString s, QString S) const
{
    for(int a = 0;a <s.count();a++)
    {
        if(s.at(a) == '/')
        {
            s.replace(a,1,S);
        }
    }
    return s;
}

QString ApplicationUI::currentMainFolderPath() const
{
    return watchFolderView->currentItem()->text(0);
}

void ApplicationUI::writeSettings()
{
    auto currentSettings = this->coreApplication->settingsState();


    this->coreApplication->setSettings(currentSettings->isCloseOnExitEnabled(),
                                       currentSettings->isRuleTimerEnabled(),
                                       currentSettings->isRulesEnabled(),
                                       this->geometry(),
                                       currentSettings->ruleTimerInterval());
}

void ApplicationUI::readSettings()
{
    auto settings = this->coreApplication->settingsState();
    setGeometry(settings->geometry());
}

void ApplicationUI::showSystemMessage(const QString &title,const QString &msg)
{
    tray->showMessage(title,msg);
}

QFont ApplicationUI::createFont(fontType ft, QString family, bool bold, bool italic, int staticSize)
{
    QFont result;
    switch(ft)
        {
            case listFont:
                {
                    screenSize <= laptopScreenSize ? result.setPointSize(12):
                                                     result.setPointSize(normalListFontSize);
                    break;
                }
            case detailedList:
                {
                    screenSize <= laptopScreenSize ? result.setPointSize(8): result.setPointSize(normalListFontSize - 4);
                    break;
                }
            case labelFont:
                {
                    screenSize <= laptopScreenSize ? result.setPointSize(10):
                                                     result.setPointSize(normalListFontSize);
                    break;
                }
            case standardFont:
                {
                    screenSize <= laptopScreenSize ? result.setPointSize(10):
                                                     result.setPointSize(normalListFontSize);
                    break;
                }
        }
    if(staticSize != -1)
        result.setPointSize(staticSize);
    result.setBold(bold);
    result.setItalic(italic);
    result.setFamily(family);
    return result;
}

void ApplicationUI::on_clearButt_clicked()
{
    auto settings = coreApplication->settingsState();
    if(settings->isRulesEnabled())
    {
        if(messageBox::customBox(this,
                                "Advarsel","Sikker på du vil gøre dette?",
                                "Ja","Nej"))
        {
            QString path = watchFolderView->currentItem()->text(0);
            coreApplication->clearFoldersAccordingToRules(QStringList(path));
        }

    }
    else
    {
        if(watchFolderView->currentItem() != nullptr &&
                messageBox::customBox(this,
                                     "Advarsel","Sikker på du vil gøre dette?",
                                     "Ja","Nej for helvede, det er hele min porno samling!"))
        {
            QString path = watchFolderView->currentItem()->text(0);
            coreApplication->clearFolders(QStringList(path));
        }
    }
}

void ApplicationUI::on_delButt_clicked()
{
    if(watchFolderView->topLevelItemCount() == 0)
        return;
    QModelIndex index = watchFolderView->currentIndex();
    if(!index.isValid())
        return;
    if(messageBox::customBox(this,tr("Sikker"),tr("Er du sikker på du vil slette posten DIT KLAMME SVIN?"),tr("Ja"),tr("Nej")))
    {
        QTreeWidgetItem *item = watchFolderView->takeTopLevelItem(index.row());
        QString path = item->text(0);
        coreApplication->removeWatchFolder(path);
    }
    updateSubTrayMenus();
    updateDetaileditems();
    suffixTree->clear();
    fileInfoBrowser->clear();
}

void ApplicationUI::updateInfoScreen(QModelIndex index)
{
    if(!index.isValid())
        return;

    QTreeWidgetItem *watchFolderItem = watchFolderView->topLevelItem(index.row());
    QString path = watchFolderItem->text(0);
    QString html = coreApplication->directoryInformationHtml(path);
    fileInfoBrowser->setHtml(html);

    suffixTree->clear();
    try {
        suffixTree->addTopLevelItems(coreApplication->suffixList(path));
    } catch (QString string) {
        setStatusText(string);
    }
}

void ApplicationUI::sortSuffixTreeColumn(int c)
{
    Qt::SortOrder suffixSortMode = suffixHeader->sortIndicatorOrder();

    if(c == 0)
    {
        if(suffixSortMode != Qt::AscendingOrder)
            suffixTree->sortByColumn(c,Qt::AscendingOrder);
        else
        {
            suffixTree->sortByColumn(c,Qt::DescendingOrder);
            suffixHeader->setSortIndicator(c,Qt::DescendingOrder);
        }
    }
    else if(c == 1)
    {
        QList<QTreeWidgetItem*>items = sortSuffixes(suffixTree,c,suffixSortMode);
        suffixTree->clear();
        suffixTree->addTopLevelItems(items);
    }
}

void ApplicationUI::updateViewIcons(QIcon ico)
{
    for(int a = 0;a <watchFolderView->topLevelItemCount();a++)
        watchFolderView->topLevelItem(a)->setIcon(0,ico);
}

QFileInfoList ApplicationUI::fileItemList(const QStringList paths) const
{
    QFileInfoList resultingList;
    for(QString path : paths)
    {
        QDir dir = QDir(path);
        resultingList += dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);
    }

    return resultingList;
}

QList<QTreeWidgetItem *> ApplicationUI::sortSuffixes(QTreeWidget *sTree, const int column, Qt::SortOrder sortMode) const
{
    unsigned long long total = static_cast<unsigned long long>(sTree->topLevelItemCount());
    QHeaderView *sufHeader = sTree->header();
    QTreeWidgetItem* unSortetItems = new QTreeWidgetItem[total],
            *sortetItems = new QTreeWidgetItem[total];
    // Initializing the unsortet array
    for (unsigned long long var = 0; var < total; ++var)
    {
        QTreeWidgetItem *sItem = sTree->topLevelItem(static_cast<int>(var));
        QTreeWidgetItem nItem;
        nItem.setText(0,sItem->text(0));
        nItem.setText(1,sItem->text(1));
        unSortetItems[var] = nItem;
    }
    sTree->clear();
    for (unsigned long long var = 0; var < total; ++var)
    {
        QTreeWidgetItem item = unSortetItems[var];
        int index = 0,
            value = item.text(column).toInt();
        for (unsigned long long i = 0; i < total; i++)
        {
            int compareValue = unSortetItems[i].text(column).toInt();
            if(sortMode != Qt::AscendingOrder)
            {
                if(i != var && value < compareValue)
                    index++;
                sufHeader->setSortIndicator(column,Qt::AscendingOrder);
            }
            else
            {
                if(i != var && value > compareValue)
                    index++;
                sufHeader->setSortIndicator(column,Qt::DescendingOrder);
            }
        }
        bool match = true;
        while(match)
        {
            if(sortetItems[index].text(column).toInt() != value)
            {
                sortetItems[index] = item;
                match = false;
            }
            else
                index++;
        }
    }

    QList<QTreeWidgetItem*>resultingSuffixes;
    for (unsigned long long var = 0; var < total; ++var)
    {
        QTreeWidgetItem item = sortetItems[var];
        QTreeWidgetItem *dupeItem = new QTreeWidgetItem;
        for (int i = 0; i < item.columnCount(); ++i)
        {
            dupeItem->setText(i,item.text(i));
        }
        resultingSuffixes << dupeItem;
    }
    return resultingSuffixes;
}

QList<QTreeWidgetItem *> ApplicationUI::sortSuffixes(QList<QTreeWidgetItem *> sItems,Qt::SortOrder order) const
{
    int total = sItems.count();
    QList<QTreeWidgetItem*>sortetItems;
    QList<int>indexList;
    for (int i = 0; i < total; ++i)
    {
        indexList.append(i);
        sortetItems.append(new QTreeWidgetItem(QStringList {"Empty","-1"}));
    }
    for (int i = 0; i < total; ++i)
    {
        QTreeWidgetItem *item = sItems.at(i);
        int index = 0,
            value = item->text(1).toInt();
        for (int j = i; j < total; j++)
        {
            int compareValue = sItems.at(j)->text(1).toInt();
            if(order != Qt::AscendingOrder && j != i && value < compareValue)
                index++;
            else if(order != Qt::DescendingOrder && j != i && value > compareValue)
                index++;
        }
        int nIndex = indexList.takeAt(index);
        sortetItems.replace(nIndex,item);
    }
    return sortetItems;
}

void ApplicationUI::updateSubTrayMenus()
{
    QStringList l = coreApplication->watchFolders();

    countTrayMenu->clear();
    clearTrayMenu->clear();
    folderTrayMenu->clear();

    if(l.isEmpty())
        return;

    countTrayMenu->addAction("Alle mapper");
    clearTrayMenu->addAction("Alle mapper");
    folderTrayMenu->addAction("Alle mapper");

    for(QString path : l)
    {
        countTrayMenu->addAction(path);
        clearTrayMenu->addAction(path);
        folderTrayMenu->addAction(path);
    }
}

void ApplicationUI::setStatusText(QString txt)
{
    statusLine->setText(txt);
    clearStatusTextTimer->start();
}

void ApplicationUI::clearStatusLine()
{
    statusLine->clear();
}

void ApplicationUI::on_actionIndstillinger_triggered()
{

     auto settingsInterface = _settingsBuilderServce->buildSettingsWindow(coreApplication);

     settingsInterface->setRulesDefinitionsService(ruleDefinitionsService());
     settingsInterface->setWidgetTitle("Settings and rules");
     QPointer<CustomDialog> dialog = new CustomDialog(settingsInterface, true);
     connect(coreApplication,&AbstractApplicationService::stateChanged,settingsInterface,&ISettingsWindow::updateView);
     dialog->show();
}

void ApplicationUI::on_actionQuit_triggered()
{
    writeSettings();
    close();
}

void ApplicationUI::on_addBut_clicked()
{
    AddFolderWidget *folderWidget = new AddFolderWidget();
    folderWidget->setWidgetTitle("Add folder dialog");
    connect(folderWidget,&AddFolderWidget::sendPath,coreApplication,&AbstractApplicationService::addWatchFolder);
    CustomDialog *dialog = new CustomDialog(folderWidget,true);
    dialog->show();
}

void ApplicationUI::on_countButt_clicked()
{
    if(coreApplication->watchFolderCount() <= 0)
        return;

    QString path = watchFolderView->currentItem()->text(0);
    coreApplication->calculateFolderSize(path);
}

void ApplicationUI::on_actionOpen_current_directory_triggered()
{
    QString cmd =  ePath + " " + modifyPath(currentDir,"\\");

    unique_ptr<QProcess>process(new QProcess(this));
    process->start(cmd);
    process->waitForFinished();
}

void ApplicationUI::updateView()
{
    updateWatchFolderView();
    updateSubTrayMenus();
    updateDetaileditems();
}

void ApplicationUI::on_detailView_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)
    if(item->childCount() >0)
        return;

    QString path = item->text(1);
    QString cmd = ePath + modifyPath(path,"\\");

    unique_ptr<QProcess>process(new QProcess(this));
    process->start(cmd);
    process->waitForFinished();
}

void ApplicationUI::on_WatchFolderView_doubleClicked(const QModelIndex &index)
{
    if(!index.isValid())
        return;
    int row = index.row();
    QTreeWidgetItem *item = watchFolderView->topLevelItem(row);
    QString p = item->text(0),
            pX = ePath + modifyPath(p,("\\"));
    if(!QFile::exists(p))
        return;

    unique_ptr<QProcess>process(new QProcess);
    process->start(pX);
    process->waitForFinished();
}
