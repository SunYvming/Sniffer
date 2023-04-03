#include "logframe.h"
#include "ui_logframe.h"
#include <iostream>

#include "layercard.h"
#include "extrainfocard.h"
#include <QVBoxLayout>

LogFrame::LogFrame(QWidget *parent) :
    QScrollArea(parent),
    ui(new Ui::LogFrame)
{
    ui->setupUi(this);
    this->widget = new QWidget(this);
    this->setWidget(widget);
    widget->setMinimumHeight(1000);
    QVBoxLayout* layout = new QVBoxLayout;
    this->widget->setLayout(layout);
}

LogFrame::~LogFrame()
{
    delete ui;
}

void LogFrame::updateLayer(uint64_t sec, uint64_t nsec, DBLoader *db)
{
    QVBoxLayout* layout = (QVBoxLayout*)this->widget->layout();
    for(auto layer : cards){
        layout->removeWidget(layer);
        delete layer;
    }
    for(auto layer : extraCards){
        layout->removeWidget(layer);
        delete layer;
    }
    cards.clear();
    extraCards.clear();
    std::vector<DBLoader::layerInfo_t> res = db->getLayers(sec,nsec);
    
    for(auto layer : res){
        LayerCard* card = new LayerCard(layer);
        cards.push_back(card);
        layout->addWidget(card);
        if(layer.type == "TCP"){
            std::vector<DBLoader::tcpInfo_t> tcpInfo = db->getTcp(sec,nsec);
            for(auto info : tcpInfo){
                
                ExtraInfoCard* tcpCard = new ExtraInfoCard(info);
                extraCards.push_back(tcpCard);
                layout->addWidget(tcpCard);
            }
        }
    }
    this->widget->setMinimumHeight(1000);
}
