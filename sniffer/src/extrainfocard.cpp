#include "extrainfocard.h"
#include "ui_tcpinfo.h"
#include "ui_tcpoption.h"
#include "ui_httpinfo.h"
#include "ui_defaultinfo.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpacerItem>

#include <string>

ExtraInfoCard::ExtraInfoCard(std::string type,QWidget *parent) :
    QFrame(parent)
{
    this->type = type;
    if(type == "TCP"){
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
    this->type = "TCP";
    ui = new Ui::TcpInfo;
    ((Ui::TcpInfo*)ui)->setupUi(this);
    ((Ui::TcpInfo*)ui)->ack->setText(QString::number(info.ack));
    ((Ui::TcpInfo*)ui)->seq->setText(QString::number(info.seq));
    ((Ui::TcpInfo*)ui)->flag->setText(QString::fromStdString(info.flags));
    ((Ui::TcpInfo*)ui)->optionsNum->setText(QString::number(info.optionNum));
    ((Ui::TcpInfo*)ui)->windowSize->setText(QString::number(info.windowSize));
}

template<typename T>
T getValueAs(size_t dataSize, size_t offset = 0, const uint8_t* data = nullptr)
{
    if (dataSize - offset < sizeof(T))
        return 0;

    T result;
    memcpy(&result, data + offset, sizeof(T));
    return result;
}

ExtraInfoCard::ExtraInfoCard(DBLoader::tcpOptionInfo_t info,QWidget *parent) :
    QFrame(parent)
{
    this->type = "TCPOption";
    ui = new Ui::TcpOption;
    ((Ui::TcpOption*)ui)->setupUi(this);
    ((Ui::TcpOption*)ui)->type->setText(QString::fromStdString(info.optionType));
    if(info.len > 0){
        if(info.optionType == "Timestamp"){
            ((Ui::TcpOption*)ui)->horizontalLayout->addWidget(new QLabel(QString::fromStdString("Timestamp:"+std::to_string(getValueAs<uint32_t>(info.len,0,info.data))),this)); 
            ((Ui::TcpOption*)ui)->horizontalLayout->addItem(new QSpacerItem(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum));
            ((Ui::TcpOption*)ui)->horizontalLayout->addWidget(new QLabel(QString::fromStdString("Echo:"+std::to_string(getValueAs<uint32_t>(info.len,4,info.data))),this));
        }
    }
    delete[] info.data;
}

ExtraInfoCard::~ExtraInfoCard()
{
    if(this->type == "TCP")
        delete ((Ui::TcpInfo*)ui);
    else if(this->type == "TCPOption")
        delete ((Ui::TcpOption*)ui);
    else if(this->type == "http")
        delete ((Ui::HttpInfo*)ui);
    else
        delete ((Ui::DefaultInfo*)ui);
}
