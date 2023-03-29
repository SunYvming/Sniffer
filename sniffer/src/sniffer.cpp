#include "sniffer.hpp"
#include "datamanager.hpp"
#include "PcapLiveDeviceList.h"
#include "exception.hpp"
#include <chrono>

void Sniffer::openDevice()
{
    if(dev->open())
        this->deviceOpened = true;
    else
    {
        this->deviceOpened = false;
        throw sniffer::DevOpenFailed();
    }
}

static void onPacketArrives(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* cookie)
{
    DataManager* dm = (DataManager*)cookie;
    pcpp::Packet parsedPacket(packet);
    dm->consumePacket(parsedPacket);
}

void Sniffer::startCapture(int timeout)
{
    try
    {
        this->openDevice();
    }
    catch(sniffer::DevOpenFailed& e)
    {
        throw sniffer::CapStartFailed(e.what());
        return;
    }

    this->dev->startCapture(onPacketArrives, this->dm);
}

void Sniffer::stopCapture()
{
    this->dev->stopCapture();
    std::cout << this->dev->getName() << std::endl;
    this->dm->printToConsole();
}