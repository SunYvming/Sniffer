#include <exception>
#include "Logger.h"
#include <string>

namespace sniffer{

    class DevOpenFailed : public std::exception
    {
        public:
            virtual const char* what() const throw() { 
                pcpp::Logger& logger = pcpp::Logger::getInstance();
                return logger.getLastError().c_str(); 
            }
    };

    class CapStartFailed : public std::exception
    {
        public:
            CapStartFailed(const char* msg) : m_msg(msg) {}
            virtual const char* what() const throw() { 
                return m_msg.c_str();
            }
        private:
            std::string m_msg;
    };

    class WithoutLayer : public std::exception
    {
        public:
            WithoutLayer(const char* msg) : m_msg(msg) {}
            virtual const char* what() const throw() { 
                return m_msg.c_str();
            }
        private:
            std::string m_msg;
    };

}