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

        void printToConsole()
        {
            std::for_each(logs.begin(), logs.end(), [](log_t log){
                std::cout << log.dlType << "->" << log.nwType << "->" << log.tpType << std::endl;
                std::cout << "From" << log.srcIp << ":"<<log.srcPort << "(" << log.srcMac<< ")" << " to " << log.dstIp << ":" << log.dstPort<< "(" << log.srcMac<< ")" << std::endl;
                std::cout << "---------------------------------" << std::endl;
            });
        }

    private:
        static SQLite::Database* db;
        std::vector<log_t> logs;

};

#endif