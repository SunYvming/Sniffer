#include "extrainfocard.h"
#include "ui_tcpinfo.h"
#include "ui_httpinfo.h"
#include "ui_defaultinfo.h"

#include <string>

ExtraInfoCard::ExtraInfoCard(std::string type,QWidget *parent) :
    QFrame(parent)
{
    this->type = type;
    if(type == "tcp"){
        ui = new Ui::TcpInfo;
        ((Ui::TcpInfo*)ui)->setupUi(this);
    }
    else if(type == "http"){
        ui = new Ui::HttpInfo;
        ((Ui::HttpInfo*)ui)->setupUi(this);
    }
    else{
        ui = new Ui::DefaultInfo;
        ((Ui::DefaultInfo*)ui)->setupUi(this);
    }
}

ExtraInfoCard::ExtraInfoCard(DBLoader::tcpInfo_t info,QWidget *parent) :
    QFrame(parent)
{
    ui = new Ui::TcpInfo;
    ((Ui::TcpInfo*)ui)->setupUi(this);
    ((Ui::TcpInfo*)ui)->ack->setText(QString::number(info.ack));
    ((Ui::TcpInfo*)ui)->seq->setText(QString::number(info.seq));
    ((Ui::TcpInfo*)ui)->flag->setText(QString::fromStdString(info.flags));
    ((Ui::TcpInfo*)ui)->optionsNum->setText(QString::number(info.optionNum));
    ((Ui::TcpInfo*)ui)->windowSize->setText(QString::number(info.windowSize));
}

ExtraInfoCard::~ExtraInfoCard()
{
    delete ui;
}
