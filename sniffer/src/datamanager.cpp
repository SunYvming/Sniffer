#include "datamanager.hpp"
#include "exception.hpp"
#include "layer.hpp"

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

void DataManager::consumePacket(pcpp::Packet &packet)
{   
    // 链接层
    pcpp::Layer *curLayer = packet.getFirstLayer();
    if(curLayer == NULL)
        throw sniffer::WithoutLayer("未找到任何协议层");

    DataLinkLayer dlLayer = DataLinkLayer(curLayer, packet);

    if(dlLayer.getType() != pcpp::Ethernet)
        return;

    // 网络层
    curLayer = curLayer->getNextLayer();
    if(curLayer == NULL)
        throw sniffer::WithoutLayer("未找到网络层");

    NetworkLayer nwLayer = NetworkLayer(curLayer, packet);
    
    // 传输层
    curLayer = curLayer->getNextLayer();
    if(curLayer == NULL)
        throw sniffer::WithoutLayer("未找到传输层");

    TransportLayer tpLayer = TransportLayer(curLayer, packet);

    // 应用层
    // 会话层和表示层不处理


    this->logs.push_back(DataManager::log_t{
        dlLayer.getSrcMac(),
        dlLayer.getDstMac(),
        getDataLinkLayerType(dlLayer.getType()),
        nwLayer.getSrcIp(),
        nwLayer.getDstIp(),
        getNetworkLayerType(nwLayer.getType()),
        tpLayer.getSrcPort(),
        tpLayer.getDstPort(),
        getTransportLayerType(tpLayer.getType())
    });
}