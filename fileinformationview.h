#ifndef FILEINFORMATIONVIEW_H
#define FILEINFORMATIONVIEW_H

#include <qgridlayout.h>

#if(_MSC_VER)
#include <QtWebEngineWidgets/QtWebEngineWidgets>
#include <iostream>
#include <QEvent>

using namespace std;
class fileInformationView : public QWidget
{
public:
    fileInformationView(QWidget *parent = 0):
        QWidget(parent)
    {
        view = new QWebEngineView(this);

        view->setHtml("<div style='height:100%;width:100%;'>"
                      "<h1>Fileinformation</h1>"
                      "</div>");
        view->setContextMenuPolicy(Qt::NoContextMenu);
        QGridLayout *grid = new QGridLayout(this);
        grid->addWidget(view);
        grid->setContentsMargins(0,0,0,0);
        view->show();
}
    void setHtml(const QString html)
    {
        view->setHtml(html);
        view->update();
    }
    void clear()
    {
        view->setHtml("<div style='height:100%;width:100%;'>"
                      "<h1>Fileinformation</h1>"
                      "</div>");
    }
    void closeWidget()
    {
        delete view;
    }
    ~fileInformationView()
    {
        delete this;
    }

protected:
    void closeEvent(QCloseEvent *event)
    {
        event->accept();
    }
private:
    QWebEngineView *view;
    QThread *wThread;
};
#elif(__MINGW32__ || __MINGW64__ || __GNUC__)
#include <qtextbrowser.h>

class fileInformationView : public QWidget
{
public:
    fileInformationView(QWidget *parent = nullptr);
    void setHtml(QString html);
    void clear();
private:
    QTextBrowser *view;
    QGridLayout *grid;
};
#endif

#endif // FILEINFORMATIONVIEW.H
