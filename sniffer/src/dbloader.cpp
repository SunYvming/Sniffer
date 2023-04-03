#include "dbloader.hpp"
#include "SQLiteCpp/SQLiteCpp.h"

DBLoader::DBLoader(std::string dev){
    this->dev = dev;
    SQLite::Database* newdb = new SQLite::Database(dev+".db", SQLite::OPEN_READONLY | SQLite::OPEN_FULLMUTEX);
    this->db = newdb;
}