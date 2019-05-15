#include "customdialog.h"


CustomDialog::CustomDialog(FrameImplementable *implementable, bool applicationModal, widget_Location location, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomDialog)
{
    ui->setupUi(this);

    widgetFrame = ui->frame;
    mainWidget = nullptr;
    topFrame = ui->TopFrame;
    grid = ui->widgetLayout;
    margin = grid->contentsMargins().bottom();

    setWindowFlag(Qt::FramelessWindowHint);

    if(applicationModal)
        setWindowModality(Qt::ApplicationModal);

    connect(topFrame,&TopFrameForm::moveParent,this,&CustomDialog::moveGlobalEvent);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    setWidget(implementable);
    show();
    setPosition(location);
}

CustomDialog::~CustomDialog()
{
    delete ui;
}

void CustomDialog::setWidget(FrameImplementable *implementable, QString title)
{

    mainWidget = implementable;
    widgetSize = mainWidget->size();
    if(grid->itemAtPosition(0,0) != nullptr)
    {
        QLayoutItem * item = grid->itemAtPosition(0,0);
        grid->removeItem(item);
    }

    connect(mainWidget,&FrameImplementable::destroyed,this,&CustomDialog::close);
    grid->addWidget(mainWidget,0,0);

    setFrameTitle(title);
}

void CustomDialog::setFrameTitle(QString title)
{
    topFrame->setFrameTitle(title);
}

QString CustomDialog::FrameTitle()
{
    return topFrame->FrameTitle();
}

void CustomDialog::on_exitButton_clicked()
{
    close();
}

void CustomDialog::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    if(mainWidget != nullptr)
        mainWidget->close();
}

void CustomDialog::mousePressEvent(QMouseEvent *event)
{
    int rightBorder = width() - margin, southBorder = height() - margin;
    if((event->pos().x() >= rightBorder && event->pos().y() >= southBorder) && mainWidget->Resizeable())
    {
        mousePressPosition = event->pos();
        tempGeometry = geometry();
        isMouseButtonPressed = true;
    }
}

void CustomDialog::mouseReleaseEvent(QMouseEvent *event)
{
    isMouseButtonPressed = false;
    setMouseTracking(true);
}

void CustomDialog::mouseMoveEvent(QMouseEvent *event)
{
    int rightBorder = width() - margin,
            lowerBorder = height() - margin;


    if((event->pos().x() >= rightBorder && event->pos().y() >= lowerBorder) && mainWidget->Resizeable())
    {
        QCursor cursor = QCursor(Qt::SizeFDiagCursor);
        setCursor(cursor);
    }
    else if(!isMouseButtonPressed) {
        QCursor cursor = QCursor(Qt::ArrowCursor);
        setCursor(cursor);
    }

    if(isMouseButtonPressed)
    {
        QPoint mousePositionDiff = event->pos() -
                QPoint(mousePressPosition.x(),mousePressPosition.y());
        int xDiff = mousePositionDiff.x(), yDiff = mousePositionDiff.y();

        QRect currentGeometry = geometry();
        int newWidth = tempGeometry.width() + xDiff,
                newHeight = tempGeometry.height() + yDiff;

        currentGeometry.setWidth(newWidth);
        currentGeometry.setHeight(newHeight);

        setGeometry(currentGeometry);
    }

}



void CustomDialog::moveGlobalEvent(QPoint pos)
{
    move(mapToParent(pos));
}

void CustomDialog::setPosition(widget_Location location)
{
    if(location == center_on_screen)
        show_centered_on_screen();
}

void CustomDialog::show_centered_on_screen()
{
    QPointer<QScreen> screen = QApplication::screens().at(0);
    QSize screenResolution = screen->size();
    int screenWidth = screenResolution.width(), screenHeight = screenResolution.height();
    double widgetX = screenWidth / 2 - widgetSize.width()/2,
            widgetY = screenHeight/2 - widgetSize.height()/2;

    QRect dialogSize = QRect(widgetX,widgetY,widgetSize.width(),widgetSize.height());
    setGeometry(dialogSize);
}
