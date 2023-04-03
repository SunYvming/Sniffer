#include "analyzewidget.h"
#include "ui_analyzewidget.h"

#include "dbloader.hpp"

#include <iostream>
#include <ctime>

void AnalyzeWidget::updateTable(std::string target){
    if(target != lastText){
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);
        lastText = target;
        for(auto db : dbs){
            if(db->getDev()==target){
                db->clearLastTime();
                break;
            }
        }
    }
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

                if(log.srcMac=="")
                    ui->tableWidget->setItem(row,2,new QTableWidgetItem(QString::fromStdString("-----")));
                else
                    ui->tableWidget->setItem(row,2,new QTableWidgetItem(QString::fromStdString(log.srcMac)));

                if(log.dstIp=="")
                    ui->tableWidget->setItem(row,3,new QTableWidgetItem(QString::fromStdString("-----")));
                else
                    ui->tableWidget->setItem(row,3,new QTableWidgetItem(QString::fromStdString(log.dstIp+":"+std::to_string(log.dstPort))));

                if(log.dstMac=="")
                    ui->tableWidget->setItem(row,4,new QTableWidgetItem(QString::fromStdString("-----")));
                else
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

                ui->tableWidget->setItem(row,8,new QTableWidgetItem(QString::fromStdString(std::to_string(log.sec))));
                ui->tableWidget->setItem(row,9,new QTableWidgetItem(QString::fromStdString(std::to_string(log.nsec))));
            }
            break;
        }
    }
}

AnalyzeWidget::AnalyzeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnalyzeWidget)
{
    ui->setupUi(this);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setColumnCount(10);
    ui->tableWidget->setColumnHidden(8,true);
    ui->tableWidget->setColumnHidden(9,true);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"时间"<<"源"<<"源mac"<<"目的"<<"目的mac"<<"网络层协议"<<"传输层协议"<<"应用层协议");
    connect(ui->update,&QPushButton::clicked,[=](){
        if(ui->comboBox->currentIndex()==-1)
        {
            ;// do nothing
        }
        else{
            updateTable(ui->comboBox->currentText().toStdString());
        }
    });
    connect(ui->comboBox,&QComboBox::currentTextChanged,[=](QString text){
        if(text==""){
            ui->tableWidget->clearContents();
            ui->tableWidget->setRowCount(0);
            lastText = "";
        }
        else
            updateTable(text.toStdString());
    });
    connect(ui->tableWidget,&QTableWidget::cellDoubleClicked,[=](int row,int col){
        uint64_t sec = ui->tableWidget->item(row,8)->text().toLong();
        uint64_t nsec = ui->tableWidget->item(row,9)->text().toLong();
        for(auto db : dbs){
            if(db->getDev()==ui->comboBox->currentText().toStdString()){
                ui->scrollArea->updateLayer(sec,nsec,db);
                break;
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
