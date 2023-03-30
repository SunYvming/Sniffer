#include "layer.hpp"

#include <string>
#include <iostream>

#include "Packet.h"
#include "Layer.h"
#include "EthLayer.h"

#include "IPv4Layer.h"
#include "IPv6Layer.h"

#include "TcpLayer.h"
#include "UdpLayer.h"

#include "exception.hpp"

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
    std::cout << "--------------------------------" << std::endl;
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
    std::cout << "--------------------------------" << std::endl;
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

void TransportLayer::printLayer()
{
    std::cout << "TransportLayer: " << getTransportLayerType(this->getType()) << std::endl;
    if(getTransportLayerType(this->getType()) == "TCP")
        std::cout << "SRC Port: " << srcPort << "DST Port: "<< dstPort <<std::endl;
    else if(getTransportLayerType(this->getType()) == "UDP")
        std::cout << "SRC Port: " << srcPort << "DST Port: "<< dstPort <<std::endl;
    std::cout << "Layer data len:" << this->getDataLen() << std::endl;
    std::cout << "--------------------------------" << std::endl;
}