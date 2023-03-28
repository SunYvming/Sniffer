#include <iostream>
#include "stdlib.h"
#include "PcapLiveDeviceList.h"
#include "SystemUtils.h"
#include <vector>

/**
* main method of the application
*/
int main(int argc, char* argv[])
{

	std::vector<pcpp::PcapLiveDevice*> allDevs = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList();

	std::for_each(allDevs.begin(), allDevs.end(), [](pcpp::PcapLiveDevice* dev) {
	std::cout
    << "Interface info:" << std::endl
    << "   Interface name:        " << dev->getName() << std::endl // get interface name
    << "   Interface description: " << dev->getDesc() << std::endl // get interface description
    << "   MAC address:           " << dev->getMacAddress() << std::endl // get interface MAC address
    << "   Default gateway:       " << dev->getDefaultGateway() << std::endl // get default gateway
    << "   Interface MTU:         " << dev->getMtu() << std::endl; // get interface MTU

	if (dev->getDnsServers().size() > 0)
		std::cout << "   DNS server:            " << dev->getDnsServers().at(0) << std::endl;
		std::cout << "   IPv4 ADDR:             " << dev->getIPv4Address().toString() << std::endl;
	});

	return 0;
}