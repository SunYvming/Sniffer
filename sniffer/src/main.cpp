#include <iostream>
#include <vector>

#include "PcapLiveDeviceList.h"
#include "SystemUtils.h"
#include "SQLiteCpp/SQLiteCpp.h"

#include "sniffer.hpp"

#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

/**
* main method of the application
*/
int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
    MainWindow w;
    w.show();

	std::vector<pcpp::PcapLiveDevice*> allDevs = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList();
	std::vector<Sniffer*> sniffers;
	std::for_each(allDevs.begin(), allDevs.end(), [&w, &sniffers](pcpp::PcapLiveDevice* dev) {
		if(dev->getMtu() != 0)
		{
			Sniffer* newSniffer = new Sniffer(dev);
			sniffers.push_back(newSniffer);
			w.addInfoCard(newSniffer);
			w.addDBLoader(newSniffer);
		}
	});

	int result = a.exec();
	// std::for_each(sniffers.begin(), sniffers.end(), [&sniffers](Sniffer *sniffer) {
	// {
	// 	delete sniffer;
	// }});	

	std::for_each(sniffers.begin(), sniffers.end(), [](Sniffer *sniffer) {
		sniffer->stopCapture();
	});
    return result;
}
