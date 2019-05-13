#include "widgetform.h"

WidgetForm::WidgetForm(QWidget *parent, bool applicationModal) :
    QWidget(),
    ui(new Ui::WidgetForm)
{
    ui->setupUi(this);

    widgetFrame = ui->frame;
    mainWidget = nullptr;
    topFrame = ui->TopFrame;
    grid = ui->widgetLayout;

    QRect fRect = widgetFrame->geometry();
    fRect.setWidth(width());

    widgetFrame->setGeometry(fRect);

    setWindowFlag(Qt::FramelessWindowHint);

    if(applicationModal)
        setWindowModality(Qt::ApplicationModal);

    setMouseTracking(true);


    connect(topFrame,&TopFrameForm::moveParent,this,&WidgetForm::moveGlobalEvent);
}

WidgetForm::~WidgetForm()
{
    delete ui;
}

void WidgetForm::setWidget(QWidget *w, QString title)
{
    mainWidget = w;

    mainWidget->setWindowFlag(Qt::FramelessWindowHint);

    if(grid->itemAtPosition(0,0) != nullptr)
    {
        QLayoutItem * item = grid->itemAtPosition(0,0);
        grid->removeItem(item);
    }

    connect(mainWidget,&QWidget::destroyed,this,&WidgetForm::close);
    grid->addWidget(w,0,0);

    setFrameTitle(title);

    adjustSizeAccordingToWidgetPolicy();    

    show();
}

void WidgetForm::setFrameTitle(QString title)
{
    topFrame->setFrameTitle(title);
}

QString WidgetForm::FrameTitle()
{
    return topFrame->FrameTitle();
}


void WidgetForm::adjustSizeAccordingToWidgetPolicy()
{

    QRect fRect = widgetFrame->geometry();
    QRect wRect = mainWidget->geometry();


    QSizePolicy wPolicy = mainWidget->sizePolicy();

    widgetFrame->setMinimumSize(mainWidget->minimumSize());
    widgetFrame->setMaximumSize(mainWidget->maximumSize());
    setMaximumWidth(mainWidget->width());
    setMinimumWidth(mainWidget->width());

    setSizePolicy(wPolicy);

    wRect.setWidth(fRect.width());
    wRect.setHeight(fRect.height());
}


void WidgetForm::on_exitButton_clicked()
{
    close();
}

void WidgetForm::closeEvent(QCloseEvent *event)
{
    if(mainWidget != nullptr)
        mainWidget->close();
}


void WidgetForm::moveGlobalEvent(QPoint pos)
{
    move(mapToParent(pos));

}
