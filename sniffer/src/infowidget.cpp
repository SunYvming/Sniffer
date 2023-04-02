#include "infowidget.h"
#include "ui_infowidget.h"

#include <QGridLayout>

#include "sniffer.hpp"
#include "infocard.h"

#include <cmath>

InfoWidget::InfoWidget(QWidget *parent) :
    QScrollArea(parent),
    ui(new Ui::InfoWidget)
{
    ui->setupUi(this);
    this->widget = new QWidget(this);
    this->setWidget(widget);
    widget->setMinimumWidth(700);
    widget->setMinimumHeight(300);
}

InfoWidget::~InfoWidget()
{
    delete ui;
}

void InfoWidget::addInfoCard(Sniffer* dev)
{
    static int counter =0;
    counter++;
    if(this->widget->layout() == nullptr)
    {
        QGridLayout* layout = new QGridLayout();
        layout->addWidget(new InfoCard(this, dev),ceil((double)counter / 2.0f),(counter%2)==1?0:1);
        // layout->addWidget(new InfoCard(this, dev));
        this->widget->setLayout(layout);
    }
    else{
        QGridLayout* layout = (QGridLayout*)this->widget->layout();
        layout->addWidget(new InfoCard(this, dev),ceil((double)counter / 2.0f),(counter%2)==1?0:1);
        // layout->addWidget(new InfoCard(this, dev));
    }
    this->widget->setMinimumHeight(360*(counter/2));
    
}
