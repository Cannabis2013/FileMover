#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include <qpoint.h>
#include <qprocess.h>
#include <qdial.h>
#include <qfont.h>
#include <qscreen.h>
#include <qstringlist.h>
#include <qheaderview.h>
#include <qtextbrowser.h>
#include <qsplitter.h>
#include <qcheckbox.h>
#include <qevent.h>
#include <qtextstream.h>
#include <qthreadpool.h>
#include <qsharedpointer.h>
#include <math.h>
#include <qstackedwidget.h>
#include <QSystemTrayIcon>
#include <iostream>
#include <qtimer.h>

#include "messageBox.h"
#include "myscreendimensions.h"
#include "fileinformationview.h"
#include "addfolderwidget.h"
#include "settingswindow.h"
#include "customdialog.h"

namespace Ui {
class ApplicationUI;
}

class ApplicationUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit ApplicationUI(AbstractApplicationService *coreApplication);
    ~ApplicationUI();

    enum fontType{listFont,detailedList,labelFont,standardFont};
    enum dateSpecifier{day,month,year};

signals:
    void StartCount(QStringList ps);
    void toLog(QString t);
    void toLog(QStringList l);
    void quit(bool iH);

protected:

    // Persistent Settings..
    void writeSettings();
    void readSettings();

    void closeEvent(QCloseEvent *cE);
    void keyPressEvent(QKeyEvent *kE);
    void keyReleaseEvent(QKeyEvent *kR);
    void mousePressEvent(QMouseEvent *mp);
    void mouseMoveEvent(QMouseEvent *e);

private slots:

    void setStatusText(QString txt);

    // Observer/observable pattern related
    void updateView();

    // Buttons..
    void on_addBut_clicked(); // Add path to "mainFolderView"
    void on_clearButt_clicked(); // The button slot that starts the "clear process"
    void on_countButt_clicked(); // Count the number of items in the selected "path"
    void on_delButt_clicked(); // Delete an item in "watchFolderView"

    // countTimer related..
    void setTimerStatus(bool makeActive);

    // DetailedFolderView related..

    void detailedFolderMenuCalled(QPoint p);
    void openFolder(bool ok);
    void on_detailView_itemDoubleClicked(QTreeWidgetItem *item, int column);

    // Fileinformation & filesuffixlist related..
    void updateInfoScreen(QModelIndex index);
    void sortSuffixTreeColumn(int c);

    // Fileworker Related..
    void clearCompleted(bool a);
    void folderContentRecieved(const DirectoryEntity *entity);
    void actionCountFolder(bool f);

    // watchFolderView Related..
    void explorerFolder(bool ok);
    void contextMenuCalled(QPoint p);
    void on_WatchFolderView_doubleClicked(const QModelIndex &index);

    // Menu actions related..
    void on_actionIndstillinger_triggered();
    void on_actionQuit_triggered();

    // settingsWindow related..
    void iconSelected(QIcon ico);

    // statusLine related..

    void clearStatusLine();

    // Sub Tray Menu Related..
    void countMenuTriggered(QAction *cAction);
    void clearMenuTriggered(QAction *clAction);
    void explorerMenuTriggered(QAction *xAction);    

    // Systemtray Related..
    void showSystemMessage(const QString &title, const QString &msg);
    void tMenuClicked(QAction *a);
    void trayClicked(QSystemTrayIcon::ActivationReason res);
    void trayMsgClicked();
    void msgToTray(const QString &msg);

    void on_actionOpen_current_directory_triggered();

private:

    // Add folder related..
    void updateViewIcons(QIcon ico);

    // Menu..
    void updateSubTrayMenus();

    //detailedFolderView related..
    void updateDetaileditems();
    void updateWatchFolderView();

    // General..
    QFont createFont(fontType ft = fontType::standardFont, QString family = "Times New Roman",bool bold = false,bool italic = false, int staticSize = -1);
    QString modifyPath(QString s,QString S) const;
    QString currentMainFolderPath() const;

    //mainFolderView related..

    QFileInfoList fileItemList(const QStringList paths) const;

    // SuffixTree related..
    QList<QTreeWidgetItem*> sortSuffixes(QTreeWidget *sTree, const int column, Qt::SortOrder sortMode = Qt::AscendingOrder) const;
    QList<QTreeWidgetItem*> sortSuffixes(QList<QTreeWidgetItem*> sItems, Qt::SortOrder order = Qt::AscendingOrder) const;

    // Systemtray related..
    QIcon currentTrayIcon() const {return tray->icon();}

    // Member variables..
    Ui::ApplicationUI *ui;

    AbstractApplicationService *coreApplication;

    bool countTimerStatus;
    fileInformationView *fileInfoBrowser;
    int tempKey,normalListFontSize, detailedListFontSize, countTimerInterval;    
    myScreenDimension laptopScreenSize, screenSize;
    QFont viewFont;    
    QHeaderView *suffixHeader;
    QIcon trayStandard,fileStandard;
    QKeyEvent *currentKeyEvent{};
    QLineEdit *statusLine;
    QList<int>columnWidths;
    QMenu *trayMenu,*countTrayMenu,*clearTrayMenu,*folderTrayMenu,*watchFolderViewMenu, *detailedFolderViewMenu;
    QModelIndex menuIndex,viewTempIndex;
    QPoint offset;
    QPushButton *clBut;
    QSplitter hSplit,vSplit;
    QStackedWidget *buttonContainer{};
    QString ePath,title,currentDir;
    QStringList directoriesToAppend;
    QSystemTrayIcon *tray;
    QTextBrowser *tBrowser{};
    QTimer *countTimer, *clearStatusTextTimer;
    QTreeWidget *detailedFolderView, *suffixTree,*watchFolderView;
    QWidget *infBox{};
};

#endif // MAINWINDOW_H
