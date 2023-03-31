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

    for (pcpp::Layer* curLayer = packet.getFirstLayer(); curLayer != NULL; curLayer = curLayer->getNextLayer())
    {
        switch (curLayer->getOsiModelLayer())
        {
            // 链路层
        case pcpp::OsiModelDataLinkLayer:{
            DataLinkLayer dlLayer = DataLinkLayer(curLayer, packet);
            dlLayer.printLayer();
        }break;

            // 网络层
        case pcpp::OsiModelNetworkLayer:{
            NetworkLayer nwLayer = NetworkLayer(curLayer, packet);
            nwLayer.printLayer();
        }break;

            // 传输层
        case pcpp::OsiModelTransportLayer:{
            TransportLayer tpLayer = TransportLayer(curLayer, packet);
            tpLayer.printLayer();
        }break;

            // 应用层
        case pcpp::OsiModelSesionLayer:
        case pcpp::OsiModelPresentationLayer:
        case pcpp::OsiModelApplicationLayer:{
            ApplicationLayer appLayer = ApplicationLayer(curLayer, packet);
            appLayer.printLayer();
        }break;
        default:
            throw sniffer::WithoutLayer("Unknown layer type");
            break;
        }
    }

    std::cout << "********************************" << std::endl;

    // this->logs.push_back(DataManager::log_t{
    //     dlLayer.getSrcMac(),
    //     dlLayer.getDstMac(),
    //     getDataLinkLayerType(dlLayer.getType()),
    //     nwLayer.getSrcIp(),
    //     nwLayer.getDstIp(),
    //     getNetworkLayerType(nwLayer.getType()),
    //     tpLayer.getSrcPort(),
    //     tpLayer.getDstPort(),
    //     getTransportLayerType(tpLayer.getType())
    // });
}