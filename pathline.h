#ifndef PATHLINE_H
#define PATHLINE_H

#include <qlineedit.h>
#include "textpathedit.h"

class PathLine : public TextPathEdit
{
    Q_OBJECT

public:
    PathLine(QWidget *parent = nullptr);

public slots:
    void setCurrentFilePath(QString fp);

signals:
    void showFileExplorer();
    void sendCurrentFilePath(QString txt);

protected:
    void on_toolButton_clicked() override;

private slots:
};

#endif // PATHLINE_H
