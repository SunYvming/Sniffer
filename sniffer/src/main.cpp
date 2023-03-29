#include <iostream>
#include "stdlib.h"
#include "PcapLiveDeviceList.h"
#include "SystemUtils.h"
#include <vector>

#include "sniffer.hpp"

/**
* main method of the application
*/
int main(int argc, char* argv[])
{

	std::vector<pcpp::PcapLiveDevice*> allDevs = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList();
	std::vector<Sniffer> sniffers;
	std::for_each(allDevs.begin(), allDevs.end(), [&sniffers](pcpp::PcapLiveDevice* dev) {
		if(dev->getMtu() != 0)
			sniffers.push_back(Sniffer(dev));
	});
	std::for_each(sniffers.begin(), sniffers.end(), [&sniffers](Sniffer &sniffer) {
		try
		{
			sniffer.printInfo();
			sniffer.startCapture();
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
			std::remove(sniffers.begin(), sniffers.end(), sniffer);
		}
	});
	// 定时捕获测试
	pcpp::multiPlatformSleep(10);
	std::for_each(sniffers.begin(), sniffers.end(), [](Sniffer &sniffer) {
		sniffer.stopCapture();
	});

	return 0;
}