#ifndef _DBLOADER_HPP_
#define _DBLOADER_HPP_

#include "SQLiteCpp/SQLiteCpp.h"
#include <vector>
#include <string>

class DBLoader
{
    public:
        typedef struct{
            uint64_t sec;
            uint64_t nsec;
            std::string dev;
            std::string srcMac;
            std::string dstMac;
            std::string dlType;
            std::string srcIp;
            std::string dstIp;
            std::string nwType;
            uint16_t srcPort;
            uint16_t dstPort;
            std::string tpType;
            uint8_t layerNum;
            std::string appType;

        }logInfo_t;

        typedef struct{
            uint64_t sec;
            uint64_t nsec;
            std::string dev;
            uint8_t layerNum;
            std::string src;
            std::string dst;
            std::string layerType;
            std::string type;
            uint16_t len;
            uint8_t* data;
        }layerInfo_t;

        DBLoader(std::string dev);
        ~DBLoader(){
            delete db;
        }

        std::string getDev(){
            return this->dev;
        }

        std::vector<logInfo_t> getAllLogs();

        std::vector<logInfo_t> getNewLogs();

        std::vector<layerInfo_t> getLayers(uint64_t sec, uint64_t nsec);

        void clearLastTime(){
            this->lastSec = 0;
            this->lastNsec = 0;
        }

    private:
        SQLite::Database* db;
        std::string dev;
        uint64_t lastSec=0;
        uint64_t lastNsec=0;
};

#endif
