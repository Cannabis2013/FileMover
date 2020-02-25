#include "customdialog.h"

CustomDialog::CustomDialog(AbstractFrame *implementable, bool applicationModal, widget_Location location, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomDialog)
{
    ui->setupUi(this);

    widgetContainer = ui->widgetContainer;
    mainWidget = nullptr;
    topFrame = ui->TopFrame;
    widgetGridLayout = ui->widgetLayout;
    mainLayout = ui->mainLayout;
    eventThreshold = 10;

    setWindowFlag(Qt::FramelessWindowHint);

    if(applicationModal)
        setWindowModality(Qt::ApplicationModal);

    connect(topFrame,&TopFrameForm::moveParent,this,&CustomDialog::moveGlobalEvent);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    setWidget(implementable);
    setPosition(location);
}

CustomDialog::~CustomDialog()
{
    delete ui;
}

void CustomDialog::setWidget(AbstractFrame *implementable)
{
    mainWidget = implementable;
    widgetSize = mainWidget->size();
    if(widgetGridLayout->itemAtPosition(0,0) != nullptr)
    {
        QLayoutItem * item = widgetGridLayout->itemAtPosition(0,0);
        widgetGridLayout->removeItem(item);
    }

    connect(mainWidget,&AbstractFrame::destroyed,this,&CustomDialog::close);
    connect(mainWidget,&AbstractFrame::sizeChanged,this,&CustomDialog::widgetSizeChanged);

    widgetGridLayout->addWidget(mainWidget,0,0);
    setFrameTitle(mainWidget->widgetTitle());
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
    int rightBorder = width() - eventThreshold, southBorder = height() - eventThreshold;
    if((event->pos().x() >= rightBorder && event->pos().y() >= southBorder) && mainWidget->resizeable())
    {
        mousePressPosition = event->pos();
        tempGeometry = geometry();
        isMouseButtonPressed = true;
    }
}

void CustomDialog::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    isMouseButtonPressed = false;
}

void CustomDialog::mouseMoveEvent(QMouseEvent *event)
{
    int rightBorder = width() - eventThreshold,
            lowerBorder = height() - eventThreshold;


    if((event->pos().x() >= rightBorder && event->pos().y() >= lowerBorder) && mainWidget->resizeable())
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

void CustomDialog::widgetSizeChanged(QSize newSize)
{
    setSize(newSize);
}

void CustomDialog::setPosition(widget_Location location)
{
    if(location == center_on_screen)
        show_centered_on_screen();
}

void CustomDialog::setSize(QSize size)
{
    int totalHeight = topFrame->height() +
            size.height() +
            widgetGridLayout->contentsMargins().bottom() + 2;

    setFixedHeight(totalHeight);
}

void CustomDialog::show_centered_on_screen()
{
    if(widgetSize == QSize())
        return;

    QPointer<QScreen> screen = QApplication::screens().at(0);
    QSize screenResolution = screen->size();
    int screenWidth = screenResolution.width(), screenHeight = screenResolution.height();
    double widgetX = screenWidth / 2 - widgetSize.width()/2,
            widgetY = screenHeight/2 - widgetSize.height()/2;

    QRect dialogSize = QRect(widgetX,widgetY,widgetSize.width(),widgetSize.height());
    setGeometry(dialogSize);
}
