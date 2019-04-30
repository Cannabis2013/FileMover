#include "mainwindow.h"
#include "ui_mainwindow.h"

mainWindow::mainWindow(AbstractCoreApplication *coreApplication,QString appName,
                       QString orgName) :
    QMainWindow(),
    AbstractPersistence(appName,orgName),
    ui(new Ui::mainWindow)
{
    ui->setupUi(this);

    // Init..
    clBut = ui->clearButt;
    clearStatusTextTimer = new QTimer();
    clearTrayMenu = new QMenu;
    columnWidths = {384,32};
    countTimer = new QTimer;
    countTimerStatus = false;
    countTrayMenu = new QMenu;
    currentDir = QDir::currentPath();
    detailedFolderView = ui->detailView;
    detailedFolderViewMenu = new QMenu();
    detailedListFontSize = 8;
    fileInfoBrowser = ui->fileInformationBrowserView;
    fileStandard = QIcon();
    folderTrayMenu = new QMenu;
    fM = new FileInformationManager();
    laptopScreenSize = myScreenDimension(0,0,1280,800);
    mainFolderView = ui->dirView;
    normalListFontSize = 16;
    screenSize = myScreenDimension(QApplication::desktop()->screenGeometry());
    statusLine = ui->statusLineEdit;
    suffixTree = ui->suffixTree;
    suffixHeader = suffixTree->header();
    tempKey = 0;
    tray = new QSystemTrayIcon();
    trayMenu = new QMenu;
    title = "FileFolderManager";
    viewFont = QFont("Times",12);
    mainFolderViewMenu = new QMenu;
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

    // Persistent Settings Related..

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

    // Fileworker Related..


    // Register meta types..

    qRegisterMetaType<QList<FileObject>>("QList<fileObject>");
    qRegisterMetaType<DirectoryItem>("directoryItem");
    qRegisterMetaType<QList<DirectoryItem>>("QList<directoryItem>");

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

}


mainWindow::~mainWindow()
{
    delete ui;
}
void mainWindow::closeEvent(QCloseEvent *cE)
{

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
                ex = ePath + modifyPath(folder(txt),"\\");
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
        QStringList list = folders();
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
    QStringList list = folders();
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

void mainWindow::folderContentRecieved(QList<FileObject> sz)
{
    QString mes, sizeString;
    foreach (FileObject fObject, sz)
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
        DirectoryItem rItem = fM->item(fObject.path);
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
}

void mainWindow::clearFolders(const QList<QTreeWidgetItem *> &itemList)
{
}

void mainWindow::countFolders()
{
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
    /*
    detailedFolderView->clear();
    QList<QTreeWidgetItem*>itemList;
    QList<directoryItem> directoryItems = fM->items();
    for(directoryItem item : directoryItems)
        itemList << item.treeWidgetItems();
    detailedFolderView->addTopLevelItems(itemList);
    */
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

QString mainWindow::currentMainFolderPath() const
{
    return mainFolderView->currentItem()->text(0);
}

void mainWindow::writeSettings()
{

}

void mainWindow::readSettings()
{
}

void mainWindow::popSystemTrayMessage(const QString msg, const QString title)
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

QStringList mainWindow::folders() const
{
    QStringList list;
    for(int a = 0;a <mainFolderView->topLevelItemCount();a++)
        list << mainFolderView->topLevelItem(a)->text(0);

    return list;
}

QString mainWindow::folder(QString t) const
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
    QStringList l = folders();

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

}

void mainWindow::on_actionQuit_triggered()
{
    writeSettings();
    close();
}

void mainWindow::on_addBut_clicked()
{
}

void mainWindow::on_countButt_clicked()
{

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
