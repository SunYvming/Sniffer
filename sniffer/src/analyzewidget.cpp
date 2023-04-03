#include "analyzewidget.h"
#include "ui_analyzewidget.h"

#include "dbloader.hpp"

#include <iostream>

AnalyzeWidget::AnalyzeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnalyzeWidget)
{
    ui->setupUi(this);
    connect(ui->update,&QPushButton::clicked,[=](){
        if(ui->comboBox->currentIndex()==-1)
        {
            ;// do nothing
        }
        else{
            // std::string target = 
            std::string target = ui->comboBox->currentText().toStdString();
            for(auto db : dbs){
                if(db->getDev()==target){
                    std::vector<DBLoader::logInfo_t> logs;
                    logs = db->getAllLogs();
                    for(auto log : logs){
                        std::cout << log.sec << "\t" << log.dev << "\tFrom" << log.srcIp+":"+std::to_string(log.srcPort) << " to " << log.dstIp+":"+std::to_string(log.dstPort) << std::endl;
                    }
                    break;
                }
            }
        }
    });
}

AnalyzeWidget::~AnalyzeWidget()
{
    delete ui;
}

void AnalyzeWidget::addDBLoader(DBLoader* db){
    dbs.push_back(db);
    ui->comboBox->addItem(QString::fromStdString(db->getDev()));
    ui->comboBox->setCurrentIndex(-1);
}
