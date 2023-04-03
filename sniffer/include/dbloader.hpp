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

        }logInfo_t;

        typedef struct{

        }layerInfo_t;

        DBLoader(std::string dev);
        ~DBLoader(){
            delete db;
        }

        std::string getDev(){
            return this->dev;
        }

        std::vector<logInfo_t> getAllLogs(){
            std::vector<logInfo_t> logs;
            logInfo_t logInfo;
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

                logs.push_back(logInfo);
            }
            return logs;
        }

    private:
        SQLite::Database* db;
        std::string dev;
};

#endif
