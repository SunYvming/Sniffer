# ifndef _SNIFFER_HPP
# define _SNIFFER_HPP

#include "PcapLiveDevice.h"
#include <string>
#include <vector>
#include <thread>

#include "datamanager.hpp"

class Sniffer
{
    public: 
        Sniffer(pcpp::PcapLiveDevice* dev)
        {
            this->dev = dev;
            this->name = dev->getName();
            this->description = dev->getDesc();
            this->mac = dev->getMacAddress();
            this->gateway = dev->getDefaultGateway();
            this->mtu = dev->getMtu();
            this->dns = dev->getDnsServers();
            this->ip = dev->getIPv4Address();

            this->dm = new DataManager();
        }

        bool operator==(const Sniffer& other)
        {
            return name == other.name;
        }

        bool isOpened()
        {
            return deviceOpened;
        }

        std::string getName()
        {
            return this->name;
        }

        void startCapture(int timeout = 0);
        void stopCapture();

        void printInfo()
        {
            std::cout << "Name: " << this->name << std::endl;
            std::cout << "Description: " << this->description << std::endl;
            std::cout << "MAC: " << this->mac.toString() << std::endl;
            std::cout << "Gateway: " << this->gateway.toString() << std::endl;
            std::cout << "MTU: " << this->mtu << std::endl;
            std::cout << "DNS: " << std::endl;
            for(auto dns : this->dns)
                std::cout << "\t" << dns.toString() << std::endl;
            std::cout << "IP: " << this->ip.toString() << std::endl;
        }

        
    private:
        pcpp::PcapLiveDevice* dev;
        std::string name;
        std::string description;
        pcpp::MacAddress mac;
        pcpp::IPv4Address gateway;
        uint32_t mtu;
        std::vector<pcpp::IPv4Address> dns;
        pcpp::IPv4Address ip;
        bool deviceOpened = false;
        void openDevice();

        DataManager* dm;
        
};

# endif