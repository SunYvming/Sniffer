#ifndef _LAYER_HPP
#define _LAYER_HPP

#include <cstring>

#include "SystemUtils.h"
#include "Packet.h"
#include "Layer.h"

#include "exception.hpp"

std::string getDataLinkLayerType(pcpp::ProtocolType protocolType);
std::string getNetworkLayerType(pcpp::ProtocolType protocolType);
std::string getTransportLayerType(pcpp::ProtocolType protocolType);

class Layer
{
public:
    Layer(pcpp::Layer *layer){
        this->dataLen = layer->getHeaderLen();
        this->data = new uint8_t[this->dataLen];
        memcpy(this->data, layer->getData(), this->dataLen);
        this->type = layer->getProtocol();
    }

    uint8_t* getData(){
        return this->data;
    }

    size_t getDataLen(){
        return this->dataLen;
    }

    uint64_t getType(){
        return this->type;
    }

    ~Layer(){
        delete[] this->data;
    }

    virtual void parse(pcpp::Packet &packet) = 0;
    virtual void printLayer() = 0;

private:
    uint8_t* data;
    size_t dataLen;
    uint64_t type;
};

class DataLinkLayer : public Layer
{
    public:
        DataLinkLayer(pcpp::Layer *layer , pcpp::Packet &packet) : Layer(layer){
            this->srcMac = "";
            this->dstMac = "";
            try
            {
                this->parse(packet);
            }
            catch(sniffer::WithoutLayer &e){
                std::cout << e.what() << std::endl;
            }
        }

        void parse(pcpp::Packet &packet) override;

        void printLayer() override;

        std::string getSrcMac(){
            return this->srcMac;
        }

        std::string getDstMac(){
            return this->dstMac;
        }

    private:
        std::string srcMac;
        std::string dstMac;

};

class NetworkLayer : public Layer
{
    public:
        NetworkLayer(pcpp::Layer *layer , pcpp::Packet &packet) : Layer(layer){
            this->srcIp = "";
            this->dstIp = "";
            try
            {
                this->parse(packet);
            }
            catch(sniffer::WithoutLayer &e){
                std::cout << e.what() << std::endl;
            }
        }

        void parse(pcpp::Packet &packet) override;

        void printLayer() override;

        std::string getSrcIp(){
            return this->srcIp;
        }

        std::string getDstIp(){
            return this->dstIp;
        }

    private:
        std::string srcIp;
        std::string dstIp;
};

class TransportLayer : public Layer
{
    public:
        TransportLayer(pcpp::Layer *layer , pcpp::Packet &packet) : Layer(layer){
            this->srcPort = 0;
            this->dstPort = 0;
            try
            {
                this->parse(packet);
            }
            catch(sniffer::WithoutLayer &e){
                std::cout << e.what() << std::endl;
            }
        }

        void parse(pcpp::Packet &packet) override;

        void printLayer() override;

        uint16_t getSrcPort(){
            return this->srcPort;
        }

        uint16_t getDstPort(){
            return this->dstPort;
        }

    private:
        uint16_t srcPort;
        uint16_t dstPort;
};



#endif