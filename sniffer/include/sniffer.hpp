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

            this->dm = new DataManager(this->name);
        }

        ~Sniffer()
        {
            delete this->dm;
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
        void printLog();

        std::string printInfo()
        {
            std::string res = 
            "Name:        \t\t" + this->name + "\n" +
            "Description: \t\t" + this->description + "\n" +
            "MAC:         \t\t" + this->mac.toString() + "\n" +
            "Gateway:     \t\t" + this->gateway.toString() + "\n" +
            "MTU:         \t\t" + std::to_string(this->mtu) + "\n" + 
            "DNS:         ";
            for(auto dns : this->dns)
                res+= "\t\t" + dns.toString() + "\n";
            res += 
            "IP:          \t\t" + this->ip.toString() + "\n";

            return res;
        }

        bool taggleCap();
        
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

        DataManager *dm;
        
};

# endif