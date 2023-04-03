#include "analyzewidget.h"
#include "ui_analyzewidget.h"

#include "dbloader.hpp"

#include <iostream>
#include <ctime>

AnalyzeWidget::AnalyzeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnalyzeWidget)
{
    ui->setupUi(this);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setColumnCount(8);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"时间"<<"源"<<"源mac"<<"目的"<<"目的mac"<<"网络层协议"<<"传输层协议"<<"应用层协议");
    connect(ui->update,&QPushButton::clicked,[=](){
        if(ui->comboBox->currentIndex()==-1)
        {
            ;// do nothing
        }
        else{
            std::string target = ui->comboBox->currentText().toStdString();
            for(auto db : dbs){
                if(db->getDev()==target){
                    std::vector<DBLoader::logInfo_t> logs;
                    logs = db->getNewLogs();
                    for(auto log : logs){
                        int row = ui->tableWidget->rowCount();
                        ui->tableWidget->insertRow(row);

                        std::time_t timestamp = log.sec;
                        std::tm* t = std::localtime(&timestamp);
                        char formatted_time[255];
                        std::strftime(formatted_time, 255, "%Y-%m-%d %H:%M:%S", t);
                        ui->tableWidget->setItem(row,0,new QTableWidgetItem(QString::fromUtf8(formatted_time)));

                        if(log.srcIp=="")
                            ui->tableWidget->setItem(row,1,new QTableWidgetItem(QString::fromStdString("-----")));
                        else
                            ui->tableWidget->setItem(row,1,new QTableWidgetItem(QString::fromStdString(log.srcIp+":"+std::to_string(log.srcPort))));
                        ui->tableWidget->setItem(row,2,new QTableWidgetItem(QString::fromStdString(log.srcMac)));

                        if(log.dstIp=="")
                            ui->tableWidget->setItem(row,3,new QTableWidgetItem(QString::fromStdString("-----")));
                        else
                            ui->tableWidget->setItem(row,3,new QTableWidgetItem(QString::fromStdString(log.dstIp+":"+std::to_string(log.dstPort))));
                        ui->tableWidget->setItem(row,4,new QTableWidgetItem(QString::fromStdString(log.dstMac)));

                        if(log.nwType=="")
                            ui->tableWidget->setItem(row,5,new QTableWidgetItem(QString::fromStdString("-----")));
                        else
                            ui->tableWidget->setItem(row,5,new QTableWidgetItem(QString::fromStdString(log.nwType)));

                        if(log.tpType=="")
                            ui->tableWidget->setItem(row,6,new QTableWidgetItem(QString::fromStdString("-----")));
                        else
                            ui->tableWidget->setItem(row,6,new QTableWidgetItem(QString::fromStdString(log.tpType)));

                        if(log.appType=="")
                            ui->tableWidget->setItem(row,7,new QTableWidgetItem(QString::fromStdString("-----")));
                        else
                            ui->tableWidget->setItem(row,7,new QTableWidgetItem(QString::fromStdString(log.appType)));
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
