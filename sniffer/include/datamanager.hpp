#ifndef _DATA_MANAGER_HPP
#define _DATA_MANAGER_HPP

#include "PcapLiveDevice.h"
#include <iostream>
#include <vector>

#include "SQLiteCpp/SQLiteCpp.h"
class DataManager
{
    public:
        typedef struct{
            std::string srcMac;
            std::string dstMac;
            std::string dlType;

            std::string srcIp;
            std::string dstIp;
            std::string nwType;

            uint16_t srcPort;
            uint16_t dstPort;
            std::string tpType;
        }log_t;
        void consumePacket(pcpp::Packet& packet);

        void insertLog();

        ~DataManager(){};

    private:
        static SQLite::Database* db;

};

#endif