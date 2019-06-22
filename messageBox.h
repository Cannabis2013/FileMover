#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <qmessagebox.h>
#include <qpushbutton.h>
#include <qlabel.h>

class messageBox
{
public:
    static bool customBox(QWidget *parent,
                          QString title,
                          QString text,
                          QString but1 = "Ok",
                          QString but2 = QString(),
                          QMessageBox::ButtonRole rol1 = QMessageBox::AcceptRole,
                          QMessageBox::ButtonRole rol2 = QMessageBox::NoRole)
    {

        QMessageBox box;
        box.setWindowTitle(title);
        box.setText(text);

        box.setStyleSheet("QMessageBox{background-color: black;border:5px solid black;border-radius:5px;} QLabel{color:white;}");

        QPushButton *b1 = box.addButton(but1,rol1);

        QPushButton *b2 = nullptr;
        if(but2 != QString())
            b2 = box.addButton(but2,rol2);

        QRect pGeo = parent->geometry();

        double parentHeight = pGeo.height() / 2,
            parentWidth = pGeo.width() / 2,
            parentX = pGeo.x(),
            parentY = pGeo.y(),
            width = box.width();

        box.exec();

        box.setGeometry(parentX + parentWidth - width/2,parentY + parentHeight,width,box.height());

        if(reinterpret_cast<QPushButton*>(box.clickedButton()) == b1)
        {
            if(rol1 == QMessageBox::AcceptRole)
                return true;
            else if(rol1 == QMessageBox::NoRole)
                return false;
            else
                return true;

        }
        else if(reinterpret_cast<QPushButton*>(box.clickedButton()) == b2)
        {
            if(rol2 == QMessageBox::AcceptRole)
                return true;
            else if(rol2 == QMessageBox::NoRole)
                return false;
            else
                return false;
        }
        else
        {
            return false;
        }
    }

};

#endif // MESSAGEBOX_H
