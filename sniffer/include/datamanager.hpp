#ifndef _DATA_MANAGER_HPP
#define _DATA_MANAGER_HPP

#include "PcapLiveDevice.h"
#include <iostream>
class DataManager
{
    public:
        void consumePacket(pcpp::Packet& packet)
        {
            if (packet.isPacketOfType(pcpp::Ethernet))
                ethPacketCount++;
            if (packet.isPacketOfType(pcpp::IPv4))
                ipv4PacketCount++;
            if (packet.isPacketOfType(pcpp::IPv6))
                ipv6PacketCount++;
            if (packet.isPacketOfType(pcpp::TCP))
                tcpPacketCount++;
            if (packet.isPacketOfType(pcpp::UDP))
                udpPacketCount++;
            if (packet.isPacketOfType(pcpp::DNS))
                dnsPacketCount++;
            if (packet.isPacketOfType(pcpp::HTTP))
                httpPacketCount++;
            if (packet.isPacketOfType(pcpp::SSL))
                sslPacketCount++;
        }

        void printToConsole()
        {
            std::cout
                << "Ethernet packet count: " << ethPacketCount << std::endl
                << "IPv4 packet count:     " << ipv4PacketCount << std::endl
                << "IPv6 packet count:     " << ipv6PacketCount << std::endl
                << "TCP packet count:      " << tcpPacketCount << std::endl
                << "UDP packet count:      " << udpPacketCount << std::endl
                << "DNS packet count:      " << dnsPacketCount << std::endl
                << "HTTP packet count:     " << httpPacketCount << std::endl
                << "SSL packet count:      " << sslPacketCount << std::endl;
        }

    private:
        int ethPacketCount;
        int ipv4PacketCount;
        int ipv6PacketCount;
        int tcpPacketCount;
        int udpPacketCount;
        int dnsPacketCount;
        int httpPacketCount;
        int sslPacketCount;

};

#endif