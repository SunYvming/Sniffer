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

DataManager::DataManager(std::string dev)
{
    this->dev = dev;
    SQLite::Database* newdb = new SQLite::Database(dev+".db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE | SQLite::OPEN_FULLMUTEX);
    // setDB(db);
    this->db = newdb;

    newdb->exec("PRAGMA journal_mode=WAL;");
    // 建表 log
    newdb->exec("CREATE TABLE IF NOT EXISTS log ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "sec BIGINT,"
            "nsec BIGINT,"
            "dev TEXT,"
            "srcMac TEXT,"
            "dstMac TEXT,"
            "dlType TEXT,"
            "srcIp TEXT,"
            "dstIp TEXT,"
            "nwType TEXT,"
            "srcPort INTEGER,"
            "dstPort INTEGER,"
            "tpType TEXT,"
            "layerNum INTEGER,"
            "appType TEXT"
            ");");
    // 建表 layer
    newdb->exec("CREATE TABLE IF NOT EXISTS layer ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "sec BIGINT,"
            "nsec BIGINT,"
            "dev TEXT,"
            "layerNum INTEGER,"
            "src TEXT,"
            "dst TEXT,"
            "layerType TEXT,"
            "type TEXT,"
            "len INTEGER,"
            "data BLOB"
            ");");
    // 建表 tcp
    newdb->exec("CREATE TABLE IF NOT EXISTS tcp ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "sec BIGINT,"
            "nsec BIGINT,"
            "dev TEXT,"
            "layerNum INTEGER,"
            "seqNum INTEGER,"
            "ackNum INTEGER,"
            "windowSize INTEGER,"
            "flags TEXT,"
            "optionNum INTEGER"
            ");");
    // 建表 tcp option
    newdb->exec("CREATE TABLE IF NOT EXISTS tcpOption ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "sec BIGINT,"
            "nsec BIGINT,"
            "dev TEXT,"
            "layerNum INTEGER,"
            "optionType TEXT,"
            "optionData BLOB"
            ");");
}

//  插入log
void DataManager::insertLog(uint64_t sec, uint64_t nsec, std::string dev,std::string srcMac, std::string dstMac, std::string dlType, std::string srcIp, std::string dstIp, std::string nwType, uint16_t srcPort, uint16_t dstPort, std::string tpType, uint8_t layerNum, std::string appType)
{
    try{
        // 插入数据
        SQLite::Statement query(*db, "INSERT INTO log (sec, nsec, dev, srcMac, dstMac, dlType, srcIp, dstIp, nwType, srcPort, dstPort, tpType, layerNum, appType) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?);");
        query.bind(1, int64_t(sec));
        query.bind(2, int64_t(nsec));
        query.bind(3, dev);
        query.bind(4, srcMac);
        query.bind(5, dstMac);
        query.bind(6, dlType);
        query.bind(7, srcIp);
        query.bind(8, dstIp);
        query.bind(9, nwType);
        query.bind(10, srcPort);
        query.bind(11, dstPort);
        query.bind(12, tpType);
        query.bind(13, layerNum);
        query.bind(14, appType);
        query.exec();
    }
    catch (std::exception& e){
        std::cout << e.what() << std::endl;
    }
}

// 插入layer
void DataManager::insertLayer(uint64_t sec, uint64_t nsec, std::string dev, uint8_t layerNum, std::string src, std::string dst, std::string layerType, std::string type, uint16_t len, uint8_t* data)
{
    try{
        // 插入数据
        SQLite::Statement query(*db, "INSERT INTO layer (sec, nsec, dev, layerNum, src, dst, layerType, type, len, data) VALUES (?,?,?,?,?,?,?,?,?,?);");
        query.bind(1, int64_t(sec));
        query.bind(2, int64_t(nsec));
        query.bind(3, dev);
        query.bind(4, layerNum);
        query.bind(5, src);
        query.bind(6, dst);
        query.bind(7, layerType);
        query.bind(8, type);
        query.bind(9, len);
        query.bind(10, data, len);
        query.exec();
    }
    catch (std::exception& e){
        std::cout << e.what() << std::endl;
    }
}

void DataManager::insertTcp(uint64_t sec, uint64_t nsec, std::string dev, uint8_t layerNum, uint32_t seqNum, uint32_t ackNum, uint16_t windowSize, std::string flags, uint8_t optionNum)
{
    try{
        // 插入数据
        SQLite::Statement query(*db, "INSERT INTO tcp (sec, nsec, dev, layerNum, seqNum, ackNum, windowSize, flags, optionNum) VALUES (?,?,?,?,?,?,?,?,?);");
        query.bind(1, int64_t(sec));
        query.bind(2, int64_t(nsec));
        query.bind(3, dev);
        query.bind(4, layerNum);
        query.bind(5, seqNum);
        query.bind(6, ackNum);
        query.bind(7, windowSize);
        query.bind(8, flags);
        query.bind(9, optionNum);
        query.exec();
    }
    catch (std::exception& e){
        std::cout << e.what() << std::endl;
    }
}

void DataManager::consumePacket(pcpp::Packet &packet)
{   
    long sec;
    long nsec;
    pcpp::clockGetTime(sec,nsec);
    std::string srcMac="";
    std::string dstMac="";
    std::string dlType="";
    std::string srcIp="";
    std::string dstIp="";
    std::string nwType="";
    uint16_t srcPort=0;
    uint16_t dstPort=0;
    std::string tpType="";
    std::string appType="";

    std::string dev = this->getDev();

    int layerNum = 0;

    SQLite::Transaction transaction(*db);
    

    for (pcpp::Layer* curLayer = packet.getFirstLayer(); curLayer != NULL; curLayer = curLayer->getNextLayer())
    {
        switch (curLayer->getOsiModelLayer())
        {
            // 链路层
        case pcpp::OsiModelDataLinkLayer:{
            DataLinkLayer dlLayer = DataLinkLayer(curLayer, packet);
            std::string layerType = "DataLinkLayer";
            uint16_t len = dlLayer.getDataLen();
            uint8_t* data = dlLayer.getData();
            if(dlLayer.getType() == pcpp::PacketTrailer){
                std::string ptType = getDataLinkLayerType(dlLayer.getType());
                insertLayer(sec, nsec, dev,layerNum, "", "", layerType, ptType, len, data);
            }
            else{
                srcMac = dlLayer.getSrcMac();
                dstMac = dlLayer.getDstMac();
                dlType = getDataLinkLayerType(dlLayer.getType());
                insertLayer(sec, nsec, dev, layerNum, srcMac, dstMac, layerType, dlType, len, data);
            }
            // dlLayer.printLayer();
        }break;

            // 网络层
        case pcpp::OsiModelNetworkLayer:{
            NetworkLayer nwLayer = NetworkLayer(curLayer, packet);
            srcIp = nwLayer.getSrcIp();
            dstIp = nwLayer.getDstIp();
            nwType = getNetworkLayerType(nwLayer.getType());
            std::string layerType = "NetworkLayer";
            uint16_t len = nwLayer.getDataLen();
            uint8_t* data = nwLayer.getData();
            insertLayer(sec, nsec, dev, layerNum, srcIp, dstIp, layerType, nwType, len, data);
            // nwLayer.printLayer();
        }break;

            // 传输层
        case pcpp::OsiModelTransportLayer:{
            TransportLayer tpLayer = TransportLayer(curLayer, packet);
            srcPort = tpLayer.getSrcPort();
            dstPort = tpLayer.getDstPort();
            tpType = getTransportLayerType(tpLayer.getType());
            if(tpType == "TCP"){
                insertTcp(sec, nsec, dev, layerNum, tpLayer.getSequenceNumber(), tpLayer.getAckNumber(), tpLayer.getWindowSize(), tpLayer.getTcpFlags(), tpLayer.getTcpOptionsNum());
            }
            std::string layerType = "TransportLayer";
            uint16_t len = tpLayer.getDataLen();
            uint8_t* data = tpLayer.getData();
            insertLayer(sec, nsec, dev, layerNum, std::to_string(srcPort), std::to_string(dstPort), layerType, tpType, len, data);
            // tpLayer.printLayer();
        }break;

            // 应用层
        case pcpp::OsiModelSesionLayer:
        case pcpp::OsiModelPresentationLayer:
        case pcpp::OsiModelApplicationLayer:{
            ApplicationLayer appLayer = ApplicationLayer(curLayer, packet);
            std::string thisAppType = getApplicationLayerType(appLayer.getType());
            std::string layerType = "ApplicationLayer";
            uint16_t len = appLayer.getDataLen();
            uint8_t* data = appLayer.getData();
            insertLayer(sec, nsec, dev, layerNum, "", "", layerType, thisAppType, len, data);
            if(appType == ""){
                appType = thisAppType;
            }
            else{
                appType = appType + " " + thisAppType;
            }
            // appLayer.printLayer();
        }break;
        default:
            throw sniffer::WithoutLayer("Unknown layer type");
            break;
        }
        layerNum++;
    }

    insertLog(sec, nsec, dev, srcMac, dstMac, dlType, srcIp, dstIp, nwType, srcPort, dstPort, tpType, layerNum, appType);

    transaction.commit();

    // std::cout << "********************************" << std::endl;
}