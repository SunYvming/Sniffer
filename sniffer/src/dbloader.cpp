#include "dbloader.hpp"
#include "SQLiteCpp/SQLiteCpp.h"
#include "cstring"

DBLoader::DBLoader(std::string dev, std::string ip){
    this->dev = dev;
    this->localIp = ip;
    SQLite::Database* newdb = new SQLite::Database(dev+".db", SQLite::OPEN_READONLY | SQLite::OPEN_FULLMUTEX);
    this->db = newdb;
}

std::vector<DBLoader::logInfo_t> DBLoader::getAllLogs(){
    std::vector<DBLoader::logInfo_t> logs;
    DBLoader::logInfo_t logInfo;
    SQLite::Statement query(*db, "SELECT * FROM log");
    while (query.executeStep())
    {
        logInfo.sec = query.getColumn(1).getInt64();
        logInfo.nsec = query.getColumn(2).getInt64();
        logInfo.dev = query.getColumn(3).getText();
        logInfo.srcMac = query.getColumn(4).getText();
        logInfo.dstMac = query.getColumn(5).getText();
        logInfo.dlType = query.getColumn(6).getText();
        logInfo.srcIp = query.getColumn(7).getText();
        logInfo.dstIp = query.getColumn(8).getText();
        logInfo.nwType = query.getColumn(9).getText();
        logInfo.srcPort = query.getColumn(10).getInt();
        logInfo.dstPort = query.getColumn(11).getInt();
        logInfo.tpType = query.getColumn(12).getText();
        logInfo.layerNum = query.getColumn(13).getInt();
        logInfo.appType = query.getColumn(14).getText();

        logs.push_back(logInfo);
    }
    return logs;
}

std::vector<DBLoader::logInfo_t> DBLoader::getNewLogs(){
    std::vector<DBLoader::logInfo_t> logs;
    DBLoader::logInfo_t logInfo;
    SQLite::Statement query(*db, "SELECT * FROM log WHERE sec > ? OR (sec = ? AND nsec > ?)");
    query.bind(1, (long)this->lastSec);
    query.bind(2, (long)this->lastSec);
    query.bind(3, (long)this->lastNsec);
    while (query.executeStep())
    {
        logInfo.sec = query.getColumn(1).getInt64();
        this->lastSec = logInfo.sec;
        logInfo.nsec = query.getColumn(2).getInt64();
        this->lastNsec = logInfo.nsec;
        logInfo.dev = query.getColumn(3).getText();
        logInfo.srcMac = query.getColumn(4).getText();
        logInfo.dstMac = query.getColumn(5).getText();
        logInfo.dlType = query.getColumn(6).getText();
        logInfo.srcIp = query.getColumn(7).getText();
        logInfo.dstIp = query.getColumn(8).getText();
        logInfo.nwType = query.getColumn(9).getText();
        logInfo.srcPort = query.getColumn(10).getInt();
        logInfo.dstPort = query.getColumn(11).getInt();
        logInfo.tpType = query.getColumn(12).getText();
        logInfo.layerNum = query.getColumn(13).getInt();
        logInfo.appType = query.getColumn(14).getText();

        logs.push_back(logInfo);
    }
    return logs;
}

std::vector<DBLoader::layerInfo_t> DBLoader::getLayers(uint64_t sec, uint64_t nsec){
    std::vector<DBLoader::layerInfo_t> layers;
    DBLoader::layerInfo_t layerInfo;
    SQLite::Statement query(*db, "SELECT * FROM layer WHERE sec = ? AND nsec = ? ORDER BY layerNum ASC");
    query.bind(1, (long)sec);
    query.bind(2, (long)nsec);
    while (query.executeStep())
    {
        layerInfo.sec = query.getColumn(1).getInt64();
        layerInfo.nsec = query.getColumn(2).getInt64();
        layerInfo.dev = query.getColumn(3).getText();
        layerInfo.layerNum = query.getColumn(4).getInt();
        layerInfo.src = query.getColumn(5).getText();
        layerInfo.dst = query.getColumn(6).getText();
        layerInfo.layerType = query.getColumn(7).getText();
        layerInfo.type = query.getColumn(8).getText();
        layerInfo.len = query.getColumn(9).getInt();
        layerInfo.data = new uint8_t[layerInfo.len];

        memcpy(layerInfo.data, query.getColumn(10).getBlob(), layerInfo.len);

        layers.push_back(layerInfo);
    }
    return layers;
}

std::vector<DBLoader::tcpInfo_t> DBLoader::getTcp(uint64_t sec, uint64_t nsec){
    std::vector<DBLoader::tcpInfo_t> tcps;
    DBLoader::tcpInfo_t tcpInfo;
    SQLite::Statement query(*db, "SELECT * FROM tcp WHERE sec = ? AND nsec = ?");
    query.bind(1, (long)sec);
    query.bind(2, (long)nsec);
    while (query.executeStep())
    {
        tcpInfo.sec = query.getColumn(1).getInt64();
        tcpInfo.nsec = query.getColumn(2).getInt64();
        tcpInfo.dev = query.getColumn(3).getText();
        tcpInfo.layerNum = query.getColumn(4).getInt();
        tcpInfo.seq = query.getColumn(5).getInt64();
        tcpInfo.ack = query.getColumn(6).getInt64();
        tcpInfo.windowSize = query.getColumn(7).getInt();
        tcpInfo.flags = query.getColumn(8).getText();
        tcpInfo.optionNum = query.getColumn(9).getInt();

        tcps.push_back(tcpInfo);
    }
    return tcps;
}

std::vector<DBLoader::tcpOptionInfo_t> DBLoader::getTcpOption(uint64_t sec, uint64_t nsec){
    std::vector<DBLoader::tcpOptionInfo_t> tcpOptions;
    DBLoader::tcpOptionInfo_t tcpOptionInfo;
    SQLite::Statement query(*db, "SELECT * FROM tcpOption WHERE sec = ? AND nsec = ?");
    query.bind(1, (long)sec);
    query.bind(2, (long)nsec);
    while (query.executeStep())
    {
        tcpOptionInfo.sec = query.getColumn(1).getInt64();
        tcpOptionInfo.nsec = query.getColumn(2).getInt64();
        tcpOptionInfo.dev = query.getColumn(3).getText();
        tcpOptionInfo.layerNum = query.getColumn(4).getInt();
        tcpOptionInfo.optionNum = query.getColumn(5).getInt();
        tcpOptionInfo.optionType = query.getColumn(6).getText();
        tcpOptionInfo.len = query.getColumn(7).getInt();
        tcpOptionInfo.data = new uint8_t[tcpOptionInfo.len];

        memcpy(tcpOptionInfo.data, query.getColumn(8).getBlob(), tcpOptionInfo.len);

        tcpOptions.push_back(tcpOptionInfo);
    }
    return tcpOptions;
}
