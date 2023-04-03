#ifndef _DATA_MANAGER_HPP
#define _DATA_MANAGER_HPP

#include "PcapLiveDevice.h"
#include <iostream>
#include <vector>

#include "SQLiteCpp/SQLiteCpp.h"
class DataManager
{
    public:
        void consumePacket(pcpp::Packet& packet);

        void insertLog(uint64_t sec, uint64_t nsec, std::string dev,std::string srcMac, std::string dstMac, std::string dlType, std::string srcIp, std::string dstIp, std::string nwType, uint16_t srcPort, uint16_t dstPort, std::string tpType, uint8_t layerNum, std::string appType);
        void insertLayer(uint64_t sec, uint64_t nsec, std::string dev, uint8_t layerNum, std::string src, std::string dst, std::string layerType, std::string type, uint16_t len, uint8_t* data);

        std::string getDev(){
            return this->dev;
        }

        DataManager(std::string dev);
        ~DataManager(){
            delete db;
        }

        void setDev(std::string dev){
            this->dev = dev;
        }

    private:
        SQLite::Database* db;
        std::string dev;

};

#endif