#include "infocard.h"
#include "ui_infocard.h"
#include "sniffer.hpp"
#include <QFrame>

InfoCard::InfoCard(QWidget *parent, Sniffer* sniffer) :
    QFrame(parent),
    ui(new Ui::InfoCard)
{
    ui->setupUi(this);
    this->sniffer = sniffer;
    if(sniffer == nullptr)
        return;
    ui->dev->setText(QString::fromStdString(sniffer->getName()));

    ui->infoValue->setText(QString::fromStdString(sniffer->printInfo()));
    setStopInfo();

    connect(ui->capCtrl, &QPushButton::clicked, [=](){
        bool state = sniffer->taggleCap();
        if(state){
            setStartInfo();
        }
        else{
            setStopInfo();
        }
    });
}

InfoCard::~InfoCard()
{
    delete ui;
    if(sniffer != nullptr)
        delete sniffer;
}

void InfoCard::setStartInfo()
{
    ui->capCtrl->setText("停止捕获");
    QPalette pal = ui->capCtrl->palette();
    pal.setColor(QPalette::Button, Qt::green);
    ui->capCtrl->setAutoFillBackground(true);
    ui->capCtrl->setPalette(pal);
    ui->capCtrl->update();
}

void InfoCard::setStopInfo()
{
    ui->capCtrl->setText("开始捕获");
    QPalette pal = ui->capCtrl->palette();
    pal.setColor(QPalette::Button, Qt::darkGray);
    ui->capCtrl->setAutoFillBackground(true);
    ui->capCtrl->setPalette(pal);
    ui->capCtrl->update();
}
