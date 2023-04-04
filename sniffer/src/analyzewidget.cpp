#include "analyzewidget.h"
#include "ui_analyzewidget.h"

#include <QComboBox>

#include "dbloader.hpp"
#include "loginfoheaderview.hpp"

#include <iostream>
#include <ctime>

void AnalyzeWidget::updateTable(std::string target){
    if(target != lastText){
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);
        lastText = target;
        for(auto db : dbs){
            if(db->getDev()==target){
                this->localIp = db->getIp();
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

                if(log.nwType=="")
                    ui->tableWidget->setItem(row,1,new QTableWidgetItem(QString::fromStdString("-----")));
                else
                    ui->tableWidget->setItem(row,1,new QTableWidgetItem(QString::fromStdString(log.nwType)));

                if(log.tpType=="")
                    ui->tableWidget->setItem(row,2,new QTableWidgetItem(QString::fromStdString("-----")));
                else
                    ui->tableWidget->setItem(row,2,new QTableWidgetItem(QString::fromStdString(log.tpType)));

                if(log.appType=="")
                    ui->tableWidget->setItem(row,3,new QTableWidgetItem(QString::fromStdString("-----")));
                else
                    ui->tableWidget->setItem(row,3,new QTableWidgetItem(QString::fromStdString(log.appType)));

                if(log.srcIp=="")
                    ui->tableWidget->setItem(row,4,new QTableWidgetItem(QString::fromStdString("-----")));
                else
                    ui->tableWidget->setItem(row,4,new QTableWidgetItem(QString::fromStdString(log.srcIp+":"+std::to_string(log.srcPort))));

                if(log.srcMac=="")
                    ui->tableWidget->setItem(row,5,new QTableWidgetItem(QString::fromStdString("-----")));
                else
                    ui->tableWidget->setItem(row,5,new QTableWidgetItem(QString::fromStdString(log.srcMac)));

                if(log.dstIp=="")
                    ui->tableWidget->setItem(row,6,new QTableWidgetItem(QString::fromStdString("-----")));
                else
                    ui->tableWidget->setItem(row,6,new QTableWidgetItem(QString::fromStdString(log.dstIp+":"+std::to_string(log.dstPort))));

                if(log.dstMac=="")
                    ui->tableWidget->setItem(row,7,new QTableWidgetItem(QString::fromStdString("-----")));
                else
                    ui->tableWidget->setItem(row,7,new QTableWidgetItem(QString::fromStdString(log.dstMac)));

                ui->tableWidget->setItem(row,8,new QTableWidgetItem(QString::fromStdString(std::to_string(log.sec))));
                ui->tableWidget->setItem(row,9,new QTableWidgetItem(QString::fromStdString(std::to_string(log.nsec))));

                std::string targetNwType = this->headerView->getNetworkFilter().toStdString();
                if(targetNwType!="全部网络层" && log.nwType.find(targetNwType) == std::string::npos){
                    // 判断类型是不是Other
                    bool isOther=true;
                    for (int i = 0; i < this->headerView->nwLayerfilter->count(); i++)
                    {
                        std::string itemText = this->headerView->nwLayerfilter->itemText(i).toStdString();
                        // 不是Other
                        if(log.nwType.find(itemText)!=std::string::npos){
                            isOther = false;
                            break;
                        }
                    }
                    if(isOther == false || (targetNwType != "Other" && isOther == true))
                        ui->tableWidget->hideRow(row);
                }

                std::string targetTpType = this->headerView->getTransportFilter().toStdString();
                if(targetTpType!="全部传输层" && targetTpType != log.tpType){
                    // 判断类型是不是Other
                    bool isOther=true;
                    for (int i = 0; i < this->headerView->tpLayerfilter->count(); i++)
                    {
                        std::string itemText = this->headerView->tpLayerfilter->itemText(i).toStdString();
                        // 不是Other
                        if(itemText == log.tpType){
                            isOther = false;
                            break;
                        }
                    }
                    if(isOther == false || (targetTpType != "Other" && isOther == true))
                        ui->tableWidget->hideRow(row);
                }

                if(this->localOnly){
                    if(log.srcIp != this->localIp && log.dstIp != this->localIp)
                        ui->tableWidget->hideRow(row);
                }
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
    
    this->headerView = new LogInfoHeaderView(1,Qt::Horizontal,ui->tableWidget);
    ui->tableWidget->setHorizontalHeader(this->headerView);
    ui->tableWidget->setColumnCount(10);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"时间"<<"网络层协议"<<"传输层协议"<<"应用层协议"<<"源"<<"源mac"<<"目的"<<"目的mac");
    ui->tableWidget->setColumnHidden(8,true);
    ui->tableWidget->setColumnHidden(9,true);

    this->localOnly = false;
    connect(ui->switchLocal,&QPushButton::clicked,[=](){
        if(localOnly){
            localOnly = false;
            ui->switchLocal->setText("仅本机");
            QPalette pal = ui->switchLocal->palette();
            pal.setColor(QPalette::Button, Qt::white);
            ui->switchLocal->setAutoFillBackground(true);
            ui->switchLocal->setPalette(pal);
            ui->switchLocal->update();
        }
        else{
            localOnly = true;
            ui->switchLocal->setText("全局");
            QPalette pal = ui->switchLocal->palette();
            pal.setColor(QPalette::Button, Qt::green);
            ui->switchLocal->setAutoFillBackground(true);
            ui->switchLocal->setPalette(pal);
            ui->switchLocal->update();
        }
        this->lastText = "";
        updateTable(ui->comboBox->currentText().toStdString());
    });

    connect(headerView->nwLayerfilter,&QComboBox::currentTextChanged,[=](QString text){
        this->lastText = "";
        updateTable(ui->comboBox->currentText().toStdString());
    });

    connect(headerView->tpLayerfilter,&QComboBox::currentTextChanged,[=](QString text){
        this->lastText = "";
        updateTable(ui->comboBox->currentText().toStdString());
    });
   
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
