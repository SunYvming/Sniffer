#include "layer.hpp"

#include <string>
#include <iostream>
#include "boost/algorithm/hex.hpp"

#include "Packet.h"
#include "Layer.h"
#include "EthLayer.h"

#include "IPv4Layer.h"
#include "IPv6Layer.h"

#include "TcpLayer.h"
#include "UdpLayer.h"

#include "HttpLayer.h"

#include "exception.hpp"

const char* field_name[FIELD_NAME_LENGTH]={
    PCPP_HTTP_HOST_FIELD,
    PCPP_HTTP_CONNECTION_FIELD,
    PCPP_HTTP_USER_AGENT_FIELD,
    PCPP_HTTP_REFERER_FIELD,
    PCPP_HTTP_ACCEPT_FIELD,
    PCPP_HTTP_ACCEPT_LANGUAGE_FIELD,
    PCPP_HTTP_ACCEPT_ENCODING_FIELD,
    PCPP_HTTP_COOKIE_FIELD,
    PCPP_HTTP_CONTENT_LENGTH_FIELD,
    PCPP_HTTP_CONTENT_TYPE_FIELD,
    PCPP_HTTP_CONTENT_ENCODING_FIELD,
    PCPP_HTTP_TRANSFER_ENCODING_FIELD,
    PCPP_HTTP_SERVER_FIELD
};

std::string getDataLinkLayerType(pcpp::ProtocolType protocolType)
{
    switch (protocolType)
    {
    case pcpp::Ethernet:
        return "Ethernet";
    case pcpp::LLC:
        return "LLC";
    case pcpp::NULL_LOOPBACK:
        return "NULL_LOOPBACK";
    case pcpp::PacketTrailer:
        return "PacketTrailer";
    case pcpp::PPPoESession:
    case pcpp::PPPoEDiscovery:
    case pcpp::PPPoE:
        return "PPPoE";
    case pcpp::SLL:
        return "SLL";
    case pcpp::STP:
        return "STP";
    case pcpp::VLAN:
        return "VLAN";
    case pcpp::VXLAN:
        return "VXLAN";
    case pcpp::WakeOnLan:
        return "WakeOnLan";
    default:
        return "Unknown";
    }
}

std::string getNetworkLayerType(pcpp::ProtocolType protocolType)
{
    switch(protocolType)
    {
    case pcpp::IPv4:
        return "IPv4";
    case pcpp::IPv6:
        return "IPv6";
    default:
        return "Unknown";
    }
}

std::string getTransportLayerType(pcpp::ProtocolType protocolType)
{
    switch(protocolType)
    {
    case pcpp::TCP:
        return "TCP";
    case pcpp::UDP:
        return "UDP";
    default:
        return "Unknown";
    }
}

std::string getApplicationLayerType(pcpp::ProtocolType protocolType)
{
    switch(protocolType)
    {
    case pcpp::HTTP:
        return "HTTP";
    default:
        return "Unknown";
    }
}

void  DataLinkLayer::parse(pcpp::Packet &packet){
    uint64_t dlType = this->getType();
    // Ethernet 协议
    if(dlType == pcpp::Ethernet)
    {
        pcpp::EthLayer *ethernetLayer = packet.getLayerOfType<pcpp::EthLayer>();
        if (ethernetLayer == NULL)
        {
            throw sniffer::WithoutLayer("链接层类型为以太网层，但未能解析以太网层");
            return;
        }
        srcMac = ethernetLayer->getSourceMac().toString();
        dstMac = ethernetLayer->getDestMac().toString();
    }
}

void DataLinkLayer::printLayer()
{
    std::cout << "DataLinkLayer: " << getDataLinkLayerType(this->getType()) << std::endl;
    if(getDataLinkLayerType(this->getType()) == "Ethernet")
        std::cout << "SRC Mac: " << srcMac << "DST Mac: "<< dstMac <<std::endl;
    std::cout << "Layer data len:" << this->getDataLen() << std::endl;
    std::cout << "Payload data len:" << this->getPayloadLen() << std::endl;
    std::cout << "================================" << std::endl;
}

void NetworkLayer::parse(pcpp::Packet &packet)
{
    uint64_t nwType = this->getType();
    // IPv4 协议
    if(nwType == pcpp::IPv4)
    {
        pcpp::IPv4Layer *ipv4Layer = packet.getLayerOfType<pcpp::IPv4Layer>();
        if (ipv4Layer == NULL)
        {
            throw sniffer::WithoutLayer("网络层类型为IPv4层，但未能解析IPv4层");
            return;
        }
        srcIp = ipv4Layer->getSrcIPAddress().toString();
        dstIp = ipv4Layer->getDstIPAddress().toString();
    }
    // IPv6 协议
    else if(nwType == pcpp::IPv6)
    {
        pcpp::IPv6Layer *ipv6Layer = packet.getLayerOfType<pcpp::IPv6Layer>();
        if (ipv6Layer == NULL)
        {
            throw sniffer::WithoutLayer("网络层类型为IPv6层，但未能解析IPv6层");
            return;
        }
        srcIp = ipv6Layer->getSrcIPAddress().toString();
        dstIp = ipv6Layer->getDstIPAddress().toString();
    }
    else{
        // 未解析的网络协议
        ;
    }
}

void NetworkLayer::printLayer()
{
    std::cout << "NetworkLayer: " << getNetworkLayerType(this->getType()) << std::endl;
    if(getNetworkLayerType(this->getType()) == "IPv4")
        std::cout << "SRC IP: " << srcIp << "DST IP: "<< dstIp <<std::endl;
    else if(getNetworkLayerType(this->getType()) == "IPv6")
        std::cout << "SRC IP: " << srcIp << "DST IP: "<< dstIp <<std::endl;
    std::cout << "Layer data len:" << this->getDataLen() << std::endl;
    std::cout << "Payload data len:" << this->getPayloadLen() << std::endl;
    std::cout << "================================" << std::endl;
}

std::string getTcpFlags(pcpp::TcpLayer* tcpLayer)
{
    std::string result = "";
    if (tcpLayer->getTcpHeader()->synFlag == 1)
        result += "SYN ";
    if (tcpLayer->getTcpHeader()->ackFlag == 1)
        result += "ACK ";
    if (tcpLayer->getTcpHeader()->pshFlag == 1)
        result += "PSH ";
    if (tcpLayer->getTcpHeader()->cwrFlag == 1)
        result += "CWR ";
    if (tcpLayer->getTcpHeader()->urgFlag == 1)
        result += "URG ";
    if (tcpLayer->getTcpHeader()->eceFlag == 1)
        result += "ECE ";
    if (tcpLayer->getTcpHeader()->rstFlag == 1)
        result += "RST ";
    if (tcpLayer->getTcpHeader()->finFlag == 1)
        result += "FIN ";

    return result;
}

void TransportLayer::parse(pcpp::Packet &packet)
{
    uint64_t tpType = this->getType();
    // TCP 协议
    if(tpType == pcpp::TCP)
    {
        pcpp::TcpLayer *tcpLayer = packet.getLayerOfType<pcpp::TcpLayer>();
        if (tcpLayer == NULL)
        {
            throw sniffer::WithoutLayer("传输层类型为TCP层，但未能解析TCP层");
            return;
        }
        srcPort = tcpLayer->getTcpHeader()->portSrc;
        dstPort = tcpLayer->getTcpHeader()->portDst;
        windowSize = tcpLayer->getTcpHeader()->windowSize;
        sequenceNumber = tcpLayer->getTcpHeader()->sequenceNumber;
        ackNumber = tcpLayer->getTcpHeader()->ackNumber;
        tcpFlags = getTcpFlags(tcpLayer);

        //解码tcp options
        for (pcpp::TcpOption tcpOption = tcpLayer->getFirstTcpOption(); tcpOption.isNotNull(); tcpOption = tcpLayer->getNextTcpOption(tcpOption))
        {
            pcpp::TcpOption* newOption = new pcpp::TcpOption(tcpOption);
            tcpOptions.push_back(newOption);
        }
    }
    // UDP 协议
    else if(tpType == pcpp::UDP)
    {
        pcpp::UdpLayer *udpLayer = packet.getLayerOfType<pcpp::UdpLayer>();
        if (udpLayer == NULL)
        {
            throw sniffer::WithoutLayer("传输层类型为UDP层，但未能解析UDP层");
            return;
        }
        srcPort = udpLayer->getUdpHeader()->portSrc;
        dstPort = udpLayer->getUdpHeader()->portDst;
    }
    else{
        // 未解析的传输协议
        ;
    }
}

std::string printTcpOptionType(pcpp::TcpOptionType optionType)
{
    switch (optionType)
    {
    case pcpp::TcpOptionType::PCPP_TCPOPT_NOP:
        return "NOP";
    case pcpp::TcpOptionType::PCPP_TCPOPT_EOL:
        return "EOL";
    case pcpp::TcpOptionType::TCPOPT_MSS:
        return "MSS";
    case pcpp::TcpOptionType::PCPP_TCPOPT_WINDOW:
        return "Window scaling";
    case pcpp::TcpOptionType::TCPOPT_SACK_PERM:
        return "SACK Permitted";
    case pcpp::TcpOptionType::TCPOPT_ECHO:
        return "Echo";
    case pcpp::TcpOptionType::TCPOPT_ECHOREPLY:
        return "Echo Reply";
    case pcpp::TcpOptionType::PCPP_TCPOPT_TIMESTAMP:
        return "Timestamp";
    case pcpp::TcpOptionType::TCPOPT_CC:
        return "CC";
    case pcpp::TcpOptionType::TCPOPT_CCNEW:
        return "CC.NEW";
    case pcpp::TcpOptionType::TCPOPT_CCECHO:
        return "CC.ECHO";
    case pcpp::TcpOptionType::TCPOPT_MD5:
        return "MD5 Signature Option";
    case pcpp::TcpOptionType::TCPOPT_MPTCP:
        return "Multipath TCP";
    case pcpp::TcpOptionType::TCPOPT_SCPS:
        return "SCPS Capabilities";
    case pcpp::TcpOptionType::TCPOPT_SNACK:
        return "SCPS SNACK";
    case pcpp::TcpOptionType::TCPOPT_RECBOUND:
        return "SCPS Record Boundary";
    case pcpp::TcpOptionType::TCPOPT_CORREXP:
        return "SCPS Corruption Experienced";
    case pcpp::TcpOptionType::TCPOPT_QS:
        return "Quick-Start Response";
    case pcpp::TcpOptionType::TCPOPT_USER_TO:
        return "User Timeout Option";
    case pcpp::TcpOptionType::TCPOPT_EXP_FD:
        return "RFC3692-style Experiment 1";
    case pcpp::TcpOptionType::TCPOPT_EXP_FE:
        return "RFC3692-style Experiment 2";
    case pcpp::TcpOptionType::TCPOPT_RVBD_PROBE:
        return "Riverbed probe option";
    case pcpp::TcpOptionType::TCPOPT_RVBD_TRPY:
        return "Riverbed transparency option";
    case pcpp::TcpOptionType::TCPOPT_Unknown:
        return "Unknown option";
    default:
        return "Unknown option";
    }
}

std::string printTcpOptions(std::vector<pcpp::TcpOption*> tcpOptions){
    std::string res = "";
    std::for_each(tcpOptions.begin(), tcpOptions.end(), [&](pcpp::TcpOption* tcpOption){
        res += "[";
        res += printTcpOptionType(tcpOption->getTcpOptionType());
        res += ":";
        uint8_t* value = tcpOption->getValue();
        if(tcpOption->getTcpOptionType() == pcpp::PCPP_TCPOPT_TIMESTAMP)
        {
            uint32_t timestamp = tcpOption->getValueAs<uint32_t>();
            uint32_t timestamp_echo = tcpOption->getValueAs<uint32_t>(4);
            res += std::to_string(timestamp);
            res += ",";
            res += std::to_string(timestamp_echo);
        }
        else if(value == NULL)
        {
            res += "NULL";
        }
        else{
            std::string str(boost::algorithm::hex(std::string(reinterpret_cast<const char*>(value), tcpOption->getDataSize())));
            res += str;
        }
        // res += tcpOption->getValueAs<std::string>();
        res += "] ";
    });
    return res;
}

void TransportLayer::printLayer()
{
    std::cout << "TransportLayer: " << getTransportLayerType(this->getType()) << std::endl;
    if(getTransportLayerType(this->getType()) == "TCP")
    {
        std::cout << "SRC Port: " << srcPort << "DST Port: "<< dstPort <<std::endl;
        std::cout << "Sequence Number: " << sequenceNumber << "Ack Number: "<< ackNumber <<std::endl;
        std::cout << "TCP Flags: " << tcpFlags <<std::endl;
        std::cout << "Window Size: " << windowSize <<std::endl;
        std::cout << "TCP Options: " << printTcpOptions(this->tcpOptions) <<std::endl;
    }
    else if(getTransportLayerType(this->getType()) == "UDP")
        std::cout << "SRC Port: " << srcPort << "DST Port: "<< dstPort <<std::endl;
    std::cout << "Layer data len:" << this->getDataLen() << std::endl;
    std::cout << "Payload data len:" << this->getPayloadLen() << std::endl;
    std::cout << "================================" << std::endl;
}

std::string printHttpMethod(pcpp::HttpRequestLayer::HttpMethod httpMethod)
{
    switch (httpMethod)
    {
    case pcpp::HttpRequestLayer::HttpGET:
        return "GET";
    case pcpp::HttpRequestLayer::HttpPOST:
        return "POST";
    default:
        return "Other";
    }
}

void ApplicationLayer::parse(pcpp::Packet &packet){
    if (this->getType() == pcpp::HTTPRequest){
        pcpp::HttpRequestLayer* httpRequestLayer = packet.getLayerOfType<pcpp::HttpRequestLayer>();
        if (httpRequestLayer == NULL)
        {
            throw sniffer::WithoutLayer("应用层类型为HTTP请求层，但未能解析HTTP请求层");
        }

        ApplicationLayer::httpData_t* data = new ApplicationLayer::httpData_t();

        data->method = printHttpMethod(httpRequestLayer->getFirstLine()->getMethod());
        data->uri = httpRequestLayer->getFirstLine()->getUri();
        data->version = httpRequestLayer->getFirstLine()->getVersion();
        data->url = httpRequestLayer->getUrl();

        for(int i=0;i < FIELD_NAME_LENGTH;i++){
            pcpp::HeaderField* field = httpRequestLayer->getFieldByName(field_name[i]);
            if(field != NULL){
                data->fields[i] = field->getFieldValue();
            }
            else{
                data->fields[i] = "(without this field)";
            }
        }

        this->data = data;
    }
    else if(this->getType() == pcpp::HTTPResponse){
        pcpp::HttpResponseLayer* httpResponseLayer = packet.getLayerOfType<pcpp::HttpResponseLayer>();
        if (httpResponseLayer == NULL)
        {
            throw sniffer::WithoutLayer("应用层类型为HTTP响应层，但未能解析HTTP响应层");
        }

        ApplicationLayer::httpData_t* data = new ApplicationLayer::httpData_t();

        data->version = httpResponseLayer->getFirstLine()->getVersion();
        data->stateCode = httpResponseLayer->getFirstLine()->getStatusCodeAsInt();
        data->stateMsg = httpResponseLayer->getFirstLine()->getStatusCodeString();

        for(int i=0;i < FIELD_NAME_LENGTH;i++){
            pcpp::HeaderField* field = httpResponseLayer->getFieldByName(field_name[i]);
            if(field != NULL){
                data->fields[i] = field->getFieldValue();
            }
            else{
                data->fields[i] = "(without this field)";
            }
        }

        this->data = data;
    }
    else{
        ;
    }
}

void ApplicationLayer::printLayer()
{
    if(this->getType()==pcpp::HTTPRequest){
        std::cout << "HTTP Request Layer" << std::endl;
        if(this->data != nullptr){
            ApplicationLayer::httpData_t* data = (ApplicationLayer::httpData_t*)(this->data);
            std::cout << "HTTP Method: " << data->method << std::endl;
            std::cout << "HTTP URI: " << data->uri << std::endl;
            std::cout << "HTTP Version: " << data->version << std::endl;
            std::cout << "HTTP URL: " << data->url << std::endl;
            for(int i=0;i < FIELD_NAME_LENGTH;i++){
                std::cout << "HTTP " << field_name[i] << ": " << data->fields[i] << std::endl;
            }
        }
    }
    else if(this->getType()==pcpp::HTTPResponse){
        std::cout << "HTTP Response Layer" << std::endl;
        if(this->data != nullptr){
            ApplicationLayer::httpData_t* data = (ApplicationLayer::httpData_t*)(this->data);
            std::cout << "HTTP Version: " << data->version << std::endl;
            std::cout << "HTTP State Code: " << data->stateCode << std::endl;
            std::cout << "HTTP State Msg: " << data->stateMsg << std::endl;
            for(int i=0;i < FIELD_NAME_LENGTH;i++){
                std::cout << "HTTP " << field_name[i] << ": " << data->fields[i] << std::endl;
            }
        }
    }
    else if(this->getType()==pcpp::HTTP){
        std::cout << "HTTP Layer" << std::endl;
    }
    else if(this->getType()==pcpp::SSL){
        std::cout << "SSL Layer" << std::endl;
    }
    else if(this->getType()==pcpp::DNS){
        std::cout << "DNS Layer" << std::endl;
    }
    else if(this->getType()==pcpp::SSH){
        std::cout << "SSH Layer" << std::endl;
    }
    else if(this->getType()==pcpp::FTP){
        std::cout << "FTP Layer" << std::endl;
    }
    else if(this->getType()==pcpp::GenericPayload){
        std::cout << "Generic Payload Layer" << std::endl;
    }
    else{
        std::cout << "Other Layer" << std::endl;
    }
    std::cout << "Layer data len:" << this->getDataLen() << std::endl;
    std::cout << "Payload data len:" << this->getPayloadLen() << std::endl;
    std::cout << "================================" << std::endl;
}