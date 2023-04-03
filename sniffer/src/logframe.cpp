#include "logframe.h"
#include "ui_logframe.h"
#include <iostream>

#include "layercard.h"
#include <QVBoxLayout>

LogFrame::LogFrame(QWidget *parent) :
    QScrollArea(parent),
    ui(new Ui::LogFrame)
{
    ui->setupUi(this);
    this->widget = new QWidget(this);
    this->setWidget(widget);
    widget->setMinimumHeight(1000);
}

LogFrame::~LogFrame()
{
    delete ui;
}

void LogFrame::updateLayer(uint64_t sec, uint64_t nsec, DBLoader *db)
{
    if(this->widget->layout() == nullptr)
    {
        QVBoxLayout* layout = new QVBoxLayout;
        std::vector<DBLoader::layerInfo_t> res = db->getLayers(sec,nsec);
        // free 旧的卡片 理论上此时没有
        for(auto layer : cards){
            delete layer;
        }
        cards.clear();
        for(auto layer : res){
            LayerCard* card = new LayerCard(layer);
            cards.push_back(card);
            layout->addWidget(card);
        }
        this->widget->setLayout(layout);
    }
    else{
        QVBoxLayout* layout = (QVBoxLayout*)this->widget->layout();
        for(auto layer : cards){
            layout->removeWidget(layer);
            delete layer;
        }
        cards.clear();
        std::vector<DBLoader::layerInfo_t> res = db->getLayers(sec,nsec);
        for(auto layer : res){
            LayerCard* card = new LayerCard(layer);
            cards.push_back(card);
            layout->addWidget(card);
        }
    }
    this->widget->setMinimumHeight(1000);
}
