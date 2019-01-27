#include "mainwindow.h"
#include "ui_mainwindow.h"

mainWindow::mainWindow(altArgsContainer args, QWidget *frameForm) :
    QMainWindow(frameForm),
    ui(new Ui::mainWindow)
{
    ui->setupUi(this);

    // Init..
    clBut = ui->clearButt;
    clearStatusTextTimer = new QTimer();
    clearTrayMenu = new QMenu;
    closeOnBut = true;
    columnWidths = {384,32};
    countTimer = new QTimer;
    countTrayMenu = new QMenu;
    currentDir = QDir::currentPath();
    dev = args.developerMode;
    detailedFolderView = ui->detailView;
    detailedFolderViewMenu = new QMenu();
    detailedListFontSize = 8;
    fileInfoBrowser = ui->fileInformationBrowserView;
    fileStandard = QIcon();
    for (int i = 0; i < 10; ++i)
        fileRuleSplitter += "-*-";
    fModel = new QFileSystemModel;
    folderTrayMenu = new QMenu;
    fM = new fileInformation();
    laptopScreenSize = myScreenDimension(0,0,1280,800);
    mainFolderView = ui->dirView;
    normalListFontSize = 16;
    pController = new processController();
    fWorker = new fW(pController);
    resMappe = "Ressources";
    ressourceDirectory = currentDir + "/" + resMappe;
    screenSize = myScreenDimension(QApplication::desktop()->screenGeometry());
    showLog = false;
    statusLine = ui->statusLineEdit;
    suffixTree = ui->suffixTree;
    suffixHeader = suffixTree->header();
    sysMappe = "fileIcons";
    tempKey = 0;
    tray = new QSystemTrayIcon();
    trayMenu = new QMenu;
    title = "FileFolderManager";
    viewFont = QFont("Times",12);
    mainFolderViewMenu = new QMenu;
    wDir = currentDir + "/" + sysMappe;
    wThread = new QThread;
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
    // Ressource Folder Related..

    QDir dir;
    if(!dir.exists(resMappe))
        dir.mkdir(resMappe);
    if(!dir.exists(sysMappe))
        dir.mkdir(sysMappe);

    // Spacer Related..

#ifdef Q_OS_MAC
    ui->verticalSpacer->changeSize(0,40,QSizePolicy::Ignored,QSizePolicy::Fixed);
#endif

    // Icon Settings..
    fileIconList = scanForIcons(sysMappe);
    if(!args.alternateIconPath.isEmpty())
        fileIconList += scanForIcons(args.alternateIconPath);

    if(!fileIconList.isEmpty())
        fileStandard = fileIconList.last();

    trayIconList = scanForIcons(resMappe);
    if(trayIconList.count() == 0)
    {
        tray->setIcon(QIcon(":/My Images/Ressources/Hdd-icon.png"));
        tray->showMessage("Info","Ingen ikoner i din ressource mappe");
    }
    else
    {
        sController.insertIcons(trayIconList);
        trayStandard = trayIconList.last();
    }
    // Persistent Settings Related..

    QCoreApplication::setApplicationName("Filehandler");
    QCoreApplication::setOrganizationName("MH");

    readSettings();

    // mainFolderView related..
    mainFolderView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    mainFolderView->setStyleSheet("QTreeWidget::branch{display:none;}");
    setWindowTitle(title);
    mainFolderView->setIconSize(QSize(32,32));

    // mainFolderView Setting columnwidth..
    mainFolderView->setColumnCount(1);
    for(int w = 0;w < mainFolderView->topLevelItemCount();w++)
    {
        int width = columnWidths.at(w);
        mainFolderView->setColumnWidth(w,width);        
    }
    mainFolderView->setHeaderHidden(true);
    mainFolderView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainFolderView->setContextMenuPolicy(Qt::CustomContextMenu);
    mainFolderView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainFolderView->setFont(createFont(fontType::listFont));
    mainFolderView->setStyleSheet("border:0px;padding:0;margin:0px;");    

    // mainFolderView Menu..

    mainFolderViewMenu->setStyleSheet("icon-size:12px;");
    mainFolderViewMenu->setTitle("MyMenu");
    mainFolderViewMenu->addAction(fileStandard,tr("Åbn mappen i explorer"),
                        this,SLOT(explorerFolder(bool)));

    // detailedFolderview related..

    QStringList headers = fW::createHeader();
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
    if(!trayIconList.isEmpty())
        tray->setIcon(trayStandard);

    // Fileworker Related..

    fWorker->moveToThread(wThread);

    // settingsWindow Related..

    sController.setCloseOnExit(closeOnBut);
    sController.setTimerEnabled(countTimer->isActive());
    sController.setTimerInterval(countTimerInterval);
    sController.setRulesEnabled(rulesEnabledStatus);

    // Register meta types..

    qRegisterMetaType<QList<fileObject>>("QList<fileObject>");
    qRegisterMetaType<directoryItem>("directoryItem");
    qRegisterMetaType<QList<directoryItem>>("QList<directoryItem>");

    // Connections..

    // Add folder related..
    connect(this,&mainWindow::processPath,fWorker,&fileWorker::processFileInformation);
    connect(this,&mainWindow::processPaths,fWorker,&fileWorker::processFileInformations);

    connect(fWorker,&fW::processFinished,this,&mainWindow::recieveDirectoryItem);
    connect(fWorker,&fW::multipleProcessFinished,this,&mainWindow::recieveDirectoryItems);

    //countTimer..
    connect(countTimer,SIGNAL(timeout()),
            this,SLOT(timerCount()));
    connect(suffixHeader,&QHeaderView::sectionDoubleClicked,
            this,&mainWindow::sortSuffixTreeColumn);

    // detailedFolderView related..
    connect(detailedFolderView,&QTreeWidget::customContextMenuRequested,
            this,&mainWindow::detailedFolderMenuCalled);

    // mainFolderView/suffixView..
    connect(mainFolderView,SIGNAL(customContextMenuRequested(QPoint)),
            this,SLOT(contextMenuCalled(QPoint)));
    connect(mainFolderView,&QTreeWidget::doubleClicked,
            this,&mainWindow::on_MainWindow_DoubleClicked);
    connect(mainFolderView,SIGNAL(pressed(QModelIndex)),
            this,SLOT(updateInfoScreen(QModelIndex)));

    // fWorker Queued Connections..
    connect(fWorker,SIGNAL(itemText(QString)),this,SLOT(setStatusText(QString)));
    connect(this,SIGNAL(StartCount(QStringList)),
            fWorker,SLOT(calcSizeOfIndividualFolderItems(QStringList)));
    connect(fWorker,SIGNAL(sendFolderContentItems(QList<fileObject>)),
            this,SLOT(folderContentRecieved(QList<fileObject>)));
    connect(fWorker,SIGNAL(clearFinished(bool)),
            this,SLOT(clearCompleted(bool)));
    connect(fWorker,SIGNAL(infoReport(QString)),
            this,SLOT(msgToTray(QString)));

    // fWorker queue related..

    connect(pController,&processController::wakeUpProcess,fWorker,&fileWorker::handleProcessRequest);

    //settingsWindow..
    /*
    */

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

    tray->show();

    //Start functions..

    // Add folders to mainFolderView..
    wThread->start();
    insertTreeItems(directoriesToAppend);

    if(frameForm != nullptr)
    {
        WidgetForm *form = static_cast<WidgetForm*>(parentWidget());
        form->setWidget(this);
    }
}


mainWindow::~mainWindow()
{
    delete ui;
}
void mainWindow::closeEvent(QCloseEvent *cE)
{
    if(sController.closeOnQuit())
    {
        fileInfoBrowser->close();
        writeSettings();
        tray->deleteLater();
        emit quit(isHidden());
        cE->accept();
    }
    else
    {
        hide();
        tray->show();
        cE->ignore();
    }
}

void mainWindow::keyPressEvent(QKeyEvent *kE)
{
    tempKey = kE->key();
}

void mainWindow::keyReleaseEvent(QKeyEvent *kR)
{
    Q_UNUSED(kR);
    tempKey = 0;
}

void mainWindow::mousePressEvent(QMouseEvent *mp)
{
    offset = mp->pos();
}

void mainWindow::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)
    {
        move(mapToParent(e->pos() - offset));
    }
}

void mainWindow::explorerMenuTriggered(QAction *xAction)
{
    QStringList arg;
    if(xAction->text() != "Alle mapper")
    {
        QString txt = xAction->text(),
        #ifdef Q_OS_WIN32
                ex = ePath + modifyPath(getItemFromList(txt),"\\");
        #elif defined Q_OS_MAC
                ex = ePath + getItemFromList(txt);
        #else
                return;
        #endif
        char *command = ex.toLocal8Bit().data();
        system(command);
    }
    else
    {
        QStringList list = getItemList();
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

void mainWindow::trayMsgClicked()
{
    show();
}

void mainWindow::msgToTray(const QString &msg)
{
    tray->showMessage("Error",msg,QSystemTrayIcon::Warning);
}

void mainWindow::setCloseonClose(bool c)
{
    closeOnBut = c;
}

void mainWindow::on_MainWindow_DoubleClicked(const QModelIndex &index)
{
    if(!index.isValid())
        return;
    int row = index.row();
    QTreeWidgetItem *item = mainFolderView->topLevelItem(row);
    QString p = item->text(0),
            pX = ePath + modifyPath(p,("\\"));
    if(!QFile::exists(p))
        return;

    unique_ptr<QProcess>process(new QProcess);
    process->start(pX);
    process->waitForFinished();
}

void mainWindow::iconSelected(QIcon ico)
{
    tray->setIcon(ico);
}

void mainWindow::trayClicked(QSystemTrayIcon::ActivationReason res)
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

void mainWindow::tMenuClicked(QAction *a)
{
    QString txt = a->text();
    if(txt == "Quit")
    {
        writeSettings();
        Quit();
    }
}

void mainWindow::countMenuTriggered(QAction *cAction)
{
    QString aText = cAction->text();
    if(aText == "Alle mapper")
    {
        QStringList allItems = getItemList();
        wThread->start();
        emit StartCount(allItems);
    }
    else
    {
        QString item = getItemFromList(aText);
        QStringList list = {item};
        wThread->start();
        StartCount(list);
    }
    wThread->start();
}

void mainWindow::clearMenuTriggered(QAction *clAction)
{
    if(clAction->text() == "Alle")
    {

    }
    else
    {

    }
}

void mainWindow::explorerFolder(bool ok)
{
    Q_UNUSED(ok);
    int r = mainFolderView->currentIndex().row();
    QStringList list = getItemList();
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

void mainWindow::folderContentRecieved(QList<fileObject> sz)
{
    QString mes, sizeString;
    foreach (fileObject fObject, sz)
    {
        QString xSize;
        double count = roundNumber(fObject.sz,xSize,2);
        if(count > 0)
        {
            sizeString = QString::number(count) + " " + xSize;
            mes += QString("'%1' indeholder %2 data! \n").arg(fObject.path,sizeString);
        }
        else
        {
            sizeString = "Tom";
            mes += QString("%1 er %2! \n").arg(fObject.path,sizeString);
        }
        directoryItem rItem = fM->getItemFromPath(fObject.path);
        rItem.dirSize = sizeString;
        fM->updateFileInfo(rItem);
    }
    tray->showMessage("Info",mes,QSystemTrayIcon::Information);
    statusLine->setText("No status. Last count: " +
                        QDate::currentDate().toString() +
                        " "
                        + QTime::currentTime().toString());
    QModelIndex cIndex = mainFolderView->currentIndex();
    updateDetaileditems();
    updateInfoScreen(cIndex);

}

void mainWindow::actionCountFolder(bool f)
{
    Q_UNUSED(f);
    if(mainFolderView->topLevelItemCount() == 0)
        return;
    QStringList list {currentMainFolderPath()};
    emit StartCount(list);
}

void mainWindow::timerCount()
{
    QStringList pathList = getItemList();
    wThread->start();
    emit StartCount(pathList);
}

void mainWindow::setTimerStatus(bool makeActive)
{
    makeActive ? countTimer->start() : countTimer->stop();
}

void mainWindow::setTimerInterval(int m)
{
    countTimerInterval = m*1000*60;
}

void mainWindow::detailedFolderMenuCalled(QPoint p)
{
    if(!detailedFolderView->currentIndex().isValid())
        return;

    QPoint P = detailedFolderView->mapToGlobal(p);
    int Y = P.y();
    P.setY(Y + 12);

    detailedFolderViewMenu->popup(P);
}

void mainWindow::openFolder(bool ok)
{
    Q_UNUSED(ok);

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

void mainWindow::clearCompleted(bool a)
{
    QString txt,msg;
    if(a)
        txt = "Succeded";
    else
        txt = "Failed";

    msg = "Folder removal " + txt;
    tray->showMessage("Removal",msg);
}

void mainWindow::contextMenuCalled(QPoint p)
{
    if(!mainFolderView->currentIndex().isValid())
        return;

    QPoint P = mainFolderView->mapToGlobal(p);
    int Y = P.y();
    P.setY(Y + 12);

    mainFolderViewMenu->popup(P);
}

void mainWindow::calcAllFiles(QStringList paths)
{
    wThread->start();
    emit StartCount(paths);
}

void mainWindow::clearFolders(const QList<QTreeWidgetItem *> &itemList)
{
    if(itemList.isEmpty())
    {
        tray->showMessage("Info","Listen er tom for mapper",
                          QSystemTrayIcon::Information,1000);
        return;
    }
    QStringList list;

    for(QTreeWidgetItem *item: itemList)
        list << item->text(0);

    if(messageBx::customBox(this,
                            "Sikker?",
                            "Sikker på du vil tømme dine valgte mapper?",
                            "ja","Nej"))
    {
        processItems it;
        it.list = fileItemList(list);
        pController->addToQueue(it);
    }
}

void mainWindow::countFolders()
{
    QStringList list = getItemList();
    wThread->start();
    emit StartCount(list);
}

void mainWindow::insertTreeItem(QString path)
{
    QStringList text {path};
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(text);
    treeItem->setIcon(0,QIcon(":/My Images/Ressources/Folder.png"));
    treeItem->font(0).setPointSize(16);
    mainFolderView->addTopLevelItem(treeItem);
}

void mainWindow::insertTreeItems(QStringList pathList)
{
    sController.insertPaths(pathList);
    emit processPaths(pathList);
    for(QString path : pathList)
    {
        QStringList text {path};
        QTreeWidgetItem *treeItem = new QTreeWidgetItem(text);
        treeItem->setIcon(0,QIcon(":/My Images/Ressources/File.png"));
        treeItem->font(0).setPointSize(16);
        mainFolderView->addTopLevelItem(treeItem);
    }
}

double mainWindow::roundNumber(long long numb, QString &denote, int dec)
{
    long long d = 1,
        dc = 10,
        sizeKb = 1024,
        sizeMb = sizeKb *1024,
        sizeGb = sizeMb * 1024;

    double byteSize = 1024, byteX, result;

    if(dec != 0)
    {
        for(int a = 0;a <dec;a++)
        {
            d = d *dc;
        }
    }
    if(numb <sizeKb)
    {
        byteX = numb;
        denote = "bytes";
    }
    else if(numb > sizeKb && numb <sizeMb)
    {
        byteX = numb / byteSize;
        denote = "kb";
    }
    else if(numb > sizeMb && numb < sizeGb)
    {
        byteX = (numb /byteSize) / byteSize;
        denote = "mb";
    }
    else
    {
        byteX = ((numb / byteSize) / byteSize) / byteSize;
        denote = "gb";
    }
    return result = round(byteX *d) / d;
}

void mainWindow::updateDetaileditems()
{
    detailedFolderView->clear();
    QList<QTreeWidgetItem*>itemList;
    QList<directoryItem> directoryItems = fM->getAllItems();
    for(directoryItem item : directoryItems)
        itemList << item.treeWidgetItems();
    detailedFolderView->addTopLevelItems(itemList);
}

QList<myIcon> mainWindow::scanForIcons(QString path)
{
    QList<myIcon>icons;
    QDirIterator iT(path);
    while(iT.hasNext())
    {
        QFileInfo file = iT.next();
#ifdef Q_OS_WIN32
        if(file.suffix() == "ico" || file.suffix() == "png")
        {
            QString fP = file.absoluteFilePath(),fN = file.fileName();
            myIcon ico(fP);
            ico.setName(fN);
            icons << ico;
        }
#elif defined Q_OS_MAC
        if(file.suffix() == "icns")
        {
            QString fP = file.absoluteFilePath(),fN = file.fileName();
            myIcon ico(fP);
            ico.setName(fN);
            icons << ico;
        }
#endif
    }
    return icons;
}

QString mainWindow::createTextBrowserHtml(const QString dirSize, const long fileCount, const int dirCount) const
{
    QString tempSize = dirSize;
    if(tempSize == QString())
        tempSize = "Not counted";
    QString fC = QString::number(fileCount),
            dC = QString::number(dirCount);
    QString resultingHtml = QString(
#if(_MSC_VER)
    QString resultingHtml = QString(                            
                            "<body style='background-color:rgb(81,81,81);'>"
                            "<table>"
                                "<td style='padding:5px;background-color:transparent;text-align:center;font-size:24px;font-weight:bold;'>"
                                    "<img src='qrc:/My Images/Ressources/File.png' width=102 height=128 >"
                                    "<p style='color:white;'>%1</p>"
                                "</td>"
                                "<td style='padding:5px;background-color:transparent;text-align:center;font-size:24px;font-weight:bold;'>"
                                    "<img src='qrc:/My Images/Ressources/Folder.png' width=102 height=128>"
                                    "<p style='color:white;'>%2</p>"
                                "</td>"
                                "<td style='padding:5px;background-color:transparent;text-align:center;font-size:24px;font-weight:bold;'>"
                                    "<img src='qrc:/My Images/Ressources/Hdd-icon.png' width=102 height=128>"
                                    "<p style='color:white;'>%3</p>"
                                "</td>"
                            "</table>"
                            "</body>").arg(fC,dC,tempSize);
#elif(__MINGW32__)
                            "<body style='background-color:rgb(81,81,81);'>"
                            "<table>"
                                "<td style='padding:5px;background-color:transparent;text-align:center;font-size:24px;font-weight:bold;'>"
                                    "<img src='qrc:/My Images/Ressources/File.png' width=102 height=128 >"
                                    "<p style='color:white;'>%1</p>"
                                "</td>"
                                "<td style='padding:5px;background-color:transparent;text-align:center;font-size:24px;font-weight:bold;'>"
                                    "<img src='qrc:/My Images/Ressources/Folder.png' width=102 height=128>"
                                    "<p style='color:white;'>%2</p>"
                                "</td>"
                                "<td style='padding:5px;background-color:transparent;text-align:center;font-size:24px;font-weight:bold;'>"
                                    "<img src='qrc:/My Images/Ressources/Hdd-icon.png' width=102 height=128>"
                                    "<p style='color:white;'>%3</p>"
                                "</td>"
                            "</table>"
                            "</body>").arg(fC,dC,tempSize);

#endif
    return resultingHtml;
}

QString mainWindow::modifyPath(QString s, QString S) const
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

QString mainWindow::mergeStringList(const QStringList &sList) const
{
    QString result = "";
    if(sList.isEmpty())
        return result;
    else if(sList.count() > 1)
        for(QString s : sList)
            result += s + ";";
    else
        return sList.at(0);
    return result;
}

QStringList mainWindow::splitString(const QString &split) const
{
    QString tempString;
    QStringList splittetList;
    int lastLetter = split.count() -1;
    for(int i = 0;i<split.count();i++)
    {
        QChar w = split.at(i);
        if(w != ';' && lastLetter != i)
            tempString.append(w);
        else
        {
            splittetList << tempString + w;
            tempString.clear();
        }
    }
    return splittetList;
}

QString mainWindow::currentMainFolderPath() const
{
    return mainFolderView->currentItem()->text(0);
}

void mainWindow::clearAccordingToRules(QStringList paths)
{
    auto freshList = [paths](QString rPath = QString(),bool recursive = false)->QFileInfoList
    {
        QStringList rPaths;
        if(rPath != QString() && rPath != "Alle")
            rPaths = QStringList(rPath);
        else
            rPaths = paths;
        QFileInfoList allFiles;
        if(!recursive)
        /*Add items to a "QFileInfoList" in a non-recursive manner,
         * which means that directories, excluding their content, will be added
         */
        {
            for(QString path : rPaths)
            {
                QDir dirContent(path);
                allFiles += dirContent.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::System);
            }
        }
        // Add files to a QFileInfoList in a recursive manner, but excludes directories
        else
        {
            for(QString path : paths)
            {
                QDirIterator it(path,
                                QDir::AllEntries | QDir::NoDotAndDotDot | QDir::System,
                                QDirIterator::Subdirectories);
                while(it.hasNext())
                {
                    QFileInfo fileItem = it.next();
                    if(!fileItem.isDir())
                        allFiles.append(fileItem);
                }
            }
        }
        return allFiles;
    };
    auto outputFiles = [](QFileInfoList list)->void
    {
        for(QFileInfo file : list)
        {
            QString fileSizeUnit;
            double sz = fW::convertSizeToAppropriateUnits(file.size(),fileSizeUnit);
            QString fileSize = QString::number(sz) + " " + fileSizeUnit;
            cout << "Filename: " << file.fileName().toStdString() << " Filepath: " << file.filePath().toStdString() <<
                 " Size: " << fileSize.toStdString() << endl;
        }
    };
    Q_UNUSED(outputFiles);
    QList<rule>rules = rController.ruleslist();
    for(rule r : rules)
    {
        QFileInfoList allFiles = freshList(r.appliesToPath,r.deepScanMode);
        for(subRule sR : r.subRules)
            allFiles = sR.processList(allFiles);

        // Implementer fileworker operation..

        processItems it;
        it.destinations = r.destinationPath;
        it.list = allFiles;
        it.ruleMode = r.actionRule;
        pController->addToQueue(it);
    }
}
void mainWindow::writeSettings()
{
    bool coe = sController.closeOnQuit(),
            countEnabled = countTimer->isActive(),
            rulesEnabled = sController.isRulesEnabled();
    QSettings s;

    s.beginGroup("Basic settings");
    s.setValue("WinGeo",QVariant(geometry()));
    s.setValue("CloseonExit",QVariant(coe));
    s.setValue("countTimerEnabled",QVariant(countEnabled));
    s.setValue("countTimerInterval",QVariant(countTimerInterval));
    s.setValue("rulesEnabled",QVariant(rulesEnabled));
    s.endGroup();

    QStringList list = getItemList();

    s.beginWriteArray("DirList",list.count());
    for(int a = 0;a <list.count();a++)
    {
        s.setArrayIndex(a);
        QString dir = list.at(a);
        s.setValue("DirPath",dir);
    }
    s.endArray();

    writeRulesToReg();
}

void mainWindow::readSettings()
{
    QSettings s;

    // Load basic settings..
    s.beginGroup("Basic Settings");
    setGeometry(s.value("WinGeo",QVariant(QRect(24,24,360,240)).toRect()).toRect());
    closeOnBut = s.value("CloseonExit",false).toBool();
    countTimerInterval = s.value("countTimerInterval",QVariant(2*1000*60).toInt()).toInt();
    countTimerStatus = s.value("countTimerEnabled",QVariant(false).toBool()).toBool();
    rulesEnabledStatus = s.value("rulesEnabled",false).toBool();
    s.endGroup();

    // Load dirlist for mainFolderView..
    int c = s.beginReadArray("DirList");
    for(int a = 0;a <c;a++)
    {
        s.setArrayIndex(a);
        directoriesToAppend << s.value("DirPath",QVariant("Error")).toString();
    }
    s.endArray();
    // Insert rules from file..
    readRulesFromReg();
}

void mainWindow::readRulesFromReg()
{
    QList<rule>rules;
    QSettings s;
    int total = s.beginReadArray("Rules");
    for (int i = 0; i < total; ++i)
    {
        rule r;
        s.setArrayIndex(i);
        r.title = s.value("Title","Title").toString();
        r.actionRule = static_cast<rD::fileActionRule>(s.value("Action","").toInt());
        r.appliesToPath = s.value("ApplyPath","Alle").toString();
        r.destinationPath = splitString(s.value("Destination paths","").toString());
        r.deepScanMode = s.value("Scan Mode",false).toBool();
        int count = s.beginReadArray("Subrules");
        for (int n = 0; n < count; ++n)
        {
            subRule sRule;
            s.setArrayIndex(n);

            sRule.copymode = static_cast<rD::copyMode>(s.value("Copymode",0).toInt());
            sRule.fieldCondition = static_cast<rD::fileFieldCondition>(s.value("Condition","").toInt());
            sRule.fileCompareMode = static_cast<rD::compareMode>(s.value("Comparemode",0).toInt());

            sRule.matchWholeWords = s.value("Matchwholewords",false).toBool();
            sRule.keyWords = splitString(s.value("Keywords","").toString());

            sRule.sizeLimit.first = s.value("Sizelimit",0).toInt();
            sRule.sizeLimit.second = s.value("Sizelimitunit","kb").toString();

            s.beginGroup("Sizelimits");
            sRule.sizeIntervalLimits.first.first = s.value("Minsizeinterval",0).toInt();
            sRule.sizeIntervalLimits.first.second = s.value("Minsizeunitinterval","kb").toString();
            sRule.sizeIntervalLimits.second.first = s.value("Maxsizeinterval",0).toInt();
            sRule.sizeIntervalLimits.second.second = s.value("Maxsizeunitinterval","kb").toString();
            s.endGroup();

            sRule.fixedDate.first = static_cast<rD::compareMode>(s.value("Comparemode",0).toInt());
            sRule.fixedDate.second = QDateTime::fromString(s.value("Datetime","").toString(),"dd.MM.yyyy");

            s.beginGroup("Datelimits");
            sRule.intervalDate.first = myDateTime::fromString(s.value("Startdate","01.01.2000").toString());
            sRule.intervalDate.second = myDateTime::fromString(s.value("Enddate","01.01.2000").toString());
            s.endGroup();

            sRule.typeMode = static_cast<wrk::iteratorMode>(s.value("Iteratormode",0).toInt());

            r.subRules.append(sRule);
        }
        s.endArray();
        rules << r;
    }
    s.endArray();
    rController.insertRules(rules);
}

void mainWindow::writeRulesToReg()
{
    QList<rule> rules = rController.ruleslist();

    QSettings s;
    s.remove("Rules");
    s.beginWriteArray("Rules",rules.count());
    for (int i = 0; i < rules.count(); ++i)
    {
        s.setArrayIndex(i);
        rule r = rules.at(i);
        s.setValue("Title",r.title);
        s.setValue("Action",r.actionRule);
        s.setValue("ApplyPath",r.appliesToPath);
        s.setValue("Destination paths",
                   mergeStringList(r.destinationPath));
        s.setValue("Scan Mode",r.deepScanMode);
        QList<subRule>sRules = r.subRules;
        int total = sRules.count();
        s.beginWriteArray("Subrules",total);
        for (int n = 0; n < total; ++n)
        {
            subRule sRule = sRules.at(n);
            s.setArrayIndex(n);

            s.setValue("Copymode",sRule.copymode);
            s.setValue("Condition",sRule.fieldCondition);
            s.setValue("Comparemode",sRule.fileCompareMode);

            s.setValue("Matchwholewords",sRule.matchWholeWords);
            s.setValue("Keywords",mergeStringList(sRule.keyWords));

            s.setValue("Sizelimit",sRule.sizeLimit.first);
            s.setValue("Sizelimitunit",sRule.sizeLimit.second);

            s.beginGroup("Sizelimits");
            s.setValue("Minsizeinterval",sRule.sizeIntervalLimits.first.first);
            s.setValue("Minsizeunitinterval",sRule.sizeIntervalLimits.first.second);
            s.setValue("Maxsizeinterval",sRule.sizeIntervalLimits.second.first);
            s.setValue("Maxsizeunitinterval",sRule.sizeIntervalLimits.second.second);
            s.endGroup();

            s.setValue("Datetime",sRule.fixedDate.second.toString("dd.MM.yyyy"));

            s.beginGroup("Datelimits");
            s.setValue("Startdate",sRule.intervalDate.first.toString("dd.MM.yyyy"));
            s.setValue("Enddate",sRule.intervalDate.second.toString("dd.MM.yyyy"));
            s.endGroup();

            s.setValue("Iteratormode",sRule.typeMode);
        }
        s.endArray();
    }
    s.endArray();
}

void mainWindow::showSystemTrayMessage(const QString msg, const QString title)
{
    tray->showMessage(title,msg);
}

QFont mainWindow::createFont(fontType ft, QString family, bool bold, bool italic, int staticSize)
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

void mainWindow::Quit()
{
    tray->deleteLater();
    bool i = isHidden();
    emit quit(i);
}

void mainWindow::on_clearButt_clicked()
{
    QList<QTreeWidgetItem*> list = mainFolderView->selectedItems();
    QStringList paths;
    for(QTreeWidgetItem*item : list)    
        paths << item->text(0);    

    if(sController.isRulesEnabled())
        clearAccordingToRules(paths);
    else
        clearFolders(list);

}


void mainWindow::on_delButt_clicked()
{
    if(mainFolderView->topLevelItemCount() == 0)
        return;
    QModelIndex index = mainFolderView->currentIndex();
    if(!index.isValid())
        return;
    if(messageBx::customBox(this,"Sikker","Er du sikker på du vil slette posten?","Ja","Nej"))
    {
        QTreeWidgetItem *item = mainFolderView->takeTopLevelItem(index.row());
        QString path = item->text(0);
        fM->removeDirectory(path);
    }
    updateSubTrayMenus();
    updateDetaileditems();
    suffixTree->clear();
    fileInfoBrowser->clear();

}

void mainWindow::updateInfoScreen(QModelIndex index)
{
    if(!index.isValid())
    {
        fileInfoBrowser->clear();
        suffixTree->clear();
        return;
    }
    QTreeWidgetItem *item = mainFolderView->topLevelItem(index.row());
    directoryItem dItem = fM->getItemFromPath(item->text(0));
    if(dItem.path == "Not defined")
        return;
    QString infoHtml = dItem.createTextBrowserHtml();

    fileInfoBrowser->setHtml(infoHtml);
    suffixTree->clear();

    QList<QTreeWidgetItem*> sortetList = sortSuffixes(dItem.suffixItems(),Qt::DescendingOrder);
    suffixTree->addTopLevelItems(sortetList);
}

void mainWindow::sortSuffixTreeColumn(int c)
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

void mainWindow::updateViewIcons(QIcon ico)
{
    for(int a = 0;a <mainFolderView->topLevelItemCount();a++)
        mainFolderView->topLevelItem(a)->setIcon(0,ico);
}

QStringList mainWindow::getItemList() const
{
    QStringList list;
    for(int a = 0;a <mainFolderView->topLevelItemCount();a++)
        list << mainFolderView->topLevelItem(a)->text(0);

    return list;
}

QString mainWindow::getItemFromList(QString t) const
{
    int count = mainFolderView->topLevelItemCount();
    for(int a = 0;a <count;a++)
    {
        QString item = mainFolderView->topLevelItem(a)->text(0);
        if(item == t)
            return item;
    }
    return QString();
}

QFileInfoList mainWindow::fileItemList(const QStringList paths) const
{
    QFileInfoList resultingList;
    for(QString path : paths)
    {
        QDir dir = QDir(path);
        resultingList += dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);
    }
    for(QFileInfo f : resultingList)
        cout << f.absoluteFilePath().toStdString() << endl;
    return resultingList;
}

QList<QTreeWidgetItem *> mainWindow::sortSuffixes(QTreeWidget *sTree, const int column, Qt::SortOrder sortMode) const
{
    uint total = static_cast<uint>(sTree->topLevelItemCount());
    QHeaderView *sufHeader = sTree->header();
    QTreeWidgetItem* unSortetItems = new QTreeWidgetItem[total],
            *sortetItems = new QTreeWidgetItem[total];
    for (int var = 0; var < static_cast<int>(total); ++var)
    {
        QTreeWidgetItem *sItem = sTree->topLevelItem(var);
        QTreeWidgetItem nItem;
        nItem.setText(0,sItem->text(0));
        nItem.setText(1,sItem->text(1));
        unSortetItems[var] = nItem;
    }
    sTree->clear();
    for (int var = 0; var < static_cast<int>(total); ++var)
    {
        QTreeWidgetItem item = unSortetItems[var];
        int index = 0,
            value = item.text(column).toInt();
        for (int i = 0; i < static_cast<int>(total); i++)
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
    for (int var = 0; var < static_cast<int>(total); ++var)
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

QList<QTreeWidgetItem *> mainWindow::sortSuffixes(QList<QTreeWidgetItem *> sItems,Qt::SortOrder order) const
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

void mainWindow::updateSubTrayMenus()
{
    QStringList l = getItemList();

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


void mainWindow::insertPath(QString p)
{
    if(fM->directoryExists(p))
    {
        showSystemTrayMessage("The folder already exists");
        return;
    }
    if(p == "C:" || p == "C:/")
    {
        showSystemTrayMessage("Rod drev ikke tilladt!");
        return;
    }
    sController.insertPath(p);
}

void mainWindow::recieveDirectoryItem(directoryItem item)
{
    fM->insertItem(item);
    updateDetaileditems();
}

void mainWindow::recieveDirectoryItems(QList<directoryItem> items)
{
    for(directoryItem item : items)
    {
        fM->insertItem(item);
    }
    updateDetaileditems();
}

void mainWindow::setStatusText(QString txt)
{
    statusLine->setText(txt);
}

void mainWindow::clearStatusLine()
{
    statusLine->clear();
}

void mainWindow::on_actionIndstillinger_triggered()
{
    sI *settingsWindow = new sI(&sController,&rController,new WidgetForm());

    connect(settingsWindow,SIGNAL(iconSelected(QIcon)),
            this,SLOT(iconSelected(QIcon)));
    connect(settingsWindow,SIGNAL(countTimerActivated(bool)),
            this,SLOT(setTimerStatus(bool)));
    connect(settingsWindow,SIGNAL(sendCheckBox(bool)),
            this,SLOT(setCloseonClose(bool)));
    connect(settingsWindow,SIGNAL(sendInterval(int)),
            this,SLOT(setTimerInterval(int)));
    connect(settingsWindow,SIGNAL(enableTimer(bool)),
            this,SLOT(setTimerStatus(bool)));

}

void mainWindow::on_actionQuit_triggered()
{
    writeSettings();
    close();
}

void mainWindow::on_addBut_clicked()
{
    addFolderWidget *addView = new addFolderWidget();
    connect(addView,&addFolderWidget::sendPath,this,&mainWindow::insertPath);
    addView->show();
}

void mainWindow::on_countButt_clicked()
{
    QList<QTreeWidgetItem*> selectedItems = mainFolderView->selectedItems();
    QStringList list;
    if(!selectedItems.isEmpty())
    {
        foreach (QTreeWidgetItem *item, selectedItems)
            list << item->text(0);
    }
    else
    {
        tray->showMessage("Info!","Vælg en sti fra oversigten!");
        return;
    }
    wThread->start();
    emit StartCount(list);
}

void mainWindow::on_actionOpen_current_directory_triggered()
{
    QString cmd =  ePath + " " + modifyPath(currentDir,"\\");

    unique_ptr<QProcess>process(new QProcess(this));
    process->start(cmd);
    process->waitForFinished();
}

void mainWindow::on_detailView_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    if(item->childCount() >0)
        return;

    QString path = item->text(1),appLaunch = "explorer ";
    QString cmd = appLaunch + modifyPath(path,"\\");

    unique_ptr<QProcess>process(new QProcess(this));
    process->start(cmd);
    process->waitForFinished();
}
