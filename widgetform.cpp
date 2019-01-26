#include "widgetform.h"

WidgetForm::WidgetForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetForm)
{
    ui->setupUi(this);

    widgetFrame = ui->frame;
    mainWidget = new QWidget(this);

    setWindowFlag(Qt::FramelessWindowHint);
    setWindowModality(Qt::ApplicationModal);
    QSizePolicy policy;
    policy.setVerticalPolicy(QSizePolicy::Minimum);
    policy.setHorizontalPolicy(QSizePolicy::Minimum);
    setSizePolicy(policy);
}

WidgetForm::~WidgetForm()
{
    delete ui;
}

void WidgetForm::setWidget(QWidget *w)
{
    mainWidget = w;

    QSizePolicy widgetSizePolicy =  mainWidget->sizePolicy();

    QGridLayout *grid = ui->widgetLayout;
    if(grid->itemAtPosition(0,0) != nullptr)
    {
        QLayoutItem * item = grid->itemAtPosition(0,0);
        grid->removeItem(item);
    }
    connect(w,&QWidget::close,this,&WidgetForm::close);
    grid->addWidget(w,0,0,Qt::AlignCenter);
}

void WidgetForm::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        move(mapToParent(event->pos() - mOffset));
    }
}

void WidgetForm::mousePressEvent(QMouseEvent *event)
{
    mOffset = event->pos();
}

void WidgetForm::setSizeWidgetSizeAccordingToPolicy(QSizePolicy &policy, QWidget *w)
{
    if(policy)
    {
        QSize fixedSize = w->size();
        setSizePolicy(policy);
        widgetFrame->setFixedSize(fixedSize);
    }
    else if(policy == QSizePolicy::Maximum)
    {

    }
    else if(policy == QSizePolicy::Minimum)
    {

    }
}


void WidgetForm::on_exitButton_clicked()
{
    close();
}

void WidgetForm::closeEvent(QEvent *event)
{

}
