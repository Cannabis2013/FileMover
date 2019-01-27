#include "widgetform.h"

WidgetForm::WidgetForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetForm)
{
    ui->setupUi(this);

    widgetFrame = ui->frame;
    mainWidget = nullptr;
    topFrame = ui->TopFrame;

    setWindowFlag(Qt::FramelessWindowHint);
    setWindowModality(Qt::ApplicationModal);
    QSizePolicy policy;
    policy.setVerticalPolicy(QSizePolicy::Minimum);
    policy.setHorizontalPolicy(QSizePolicy::Minimum);
    setSizePolicy(policy);

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
    adjustSizeAccordingToWidgetPolicy();

    QGridLayout *grid = ui->mainLayout;
    if(grid->itemAtPosition(0,0) != nullptr)
    {
        QLayoutItem * item = grid->itemAtPosition(0,0);
        grid->removeItem(item);
    }

    connect(mainWidget,&QWidget::destroyed,this,&WidgetForm::close);
    grid->addWidget(w,0,0,Qt::AlignCenter);

    setFrameTitle(title);

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
    setSizePolicy(mainWidget->sizePolicy());
    widgetFrame->setMinimumSize(mainWidget->minimumSize());
    widgetFrame->setMaximumSize(mainWidget->maximumSize());
    setMaximumWidth(mainWidget->width());
    setMinimumWidth(mainWidget->width());
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
