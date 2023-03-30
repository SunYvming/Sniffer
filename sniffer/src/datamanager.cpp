#include "datamanager.hpp"
#include "exception.hpp"

#include "SQLiteCpp/SQLiteCpp.h"

#include "SystemUtils.h"
#include "Packet.h"
#include "EthLayer.h"

#include "IPv4Layer.h"
#include "IPv6Layer.h"

#include "TcpLayer.h"
#include "UdpLayer.h"
// #include "PcapLiveDeviceList.h"

//  测试用简化版
void DataManager::insertLog()
{
    // 若数据库未打开，打开数据库
    // 若表log没有，建表
    // 若表ethernet没有，建表
}

std::string getDataLinkLayerType(pcpp::ProtocolType protocolType)
{
    switch (protocolType)
    {
    case pcpp::Ethernet:
        return "Ethernet";
    // case pcpp::LLC:
    //     return "LLC";
    // case pcpp::NULL_LOOPBACK:
    //     return "NULL_LOOPBACK";
    // case pcpp::PacketTrailer:
    //     return "PacketTrailer";
    // case pcpp::PPPoESession:
    // case pcpp::PPPoEDiscovery:
    // case pcpp::PPPoE:
    //     return "PPPoE";
    // case pcpp::SLL:
    //     return "SLL";
    // case pcpp::STP:
    //     return "STP";
    // case pcpp::VLAN:
    //     return "VLAN";
    // case pcpp::VXLAN:
    //     return "VXLAN";
    // case pcpp::WakeOnLan:
    //     return "WakeOnLan";
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

void DataManager::consumePacket(pcpp::Packet &packet)
{   
    // 链接层
    pcpp::Layer *curLayer = packet.getFirstLayer();
    if(curLayer == NULL)
    {
        throw sniffer::WithoutLayer("未找到任何协议层");
        return;
    }
    uint64_t dlType = curLayer->getProtocol();
    std::string srcMac="";
    std::string dstMac="";
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

    // 网络层
    curLayer = curLayer->getNextLayer();
    if(curLayer == NULL)
    {
        throw sniffer::WithoutLayer("未找到网络层");
        return;
    }
    uint64_t nwType = curLayer->getProtocol();
    std::string srcIp="";
    std::string dstIp="";
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
        // 不支持的网络层类型
        ;
    }

    // 传输层
    curLayer = curLayer->getNextLayer();
    if(curLayer == NULL)
    {
        throw sniffer::WithoutLayer("未找到传输层");
        return;
    }
    uint64_t tpType = curLayer->getProtocol();
    uint16_t srcPort=0;
    uint16_t dstPort=0;
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
        // 不支持的传输层类型
        ;
    }

    // 应用层
    // 会话层和表示层不处理


    this->logs.push_back(DataManager::log_t{
        srcMac,
        dstMac,
        getDataLinkLayerType(dlType),
        srcIp,
        dstIp,
        getNetworkLayerType(nwType),
        srcPort,
        dstPort,
        getTransportLayerType(tpType)
    });
}