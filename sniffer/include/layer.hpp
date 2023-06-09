#ifndef _LAYER_HPP
#define _LAYER_HPP

#include <cstring>
#include <vector>
#include <algorithm>

#include "SystemUtils.h"
#include "Packet.h"
#include "Layer.h"
#include "TcpLayer.h"
#include "HttpLayer.h"

#include "exception.hpp"

#define FIELD_NAME_LENGTH 13

extern const char* field_name[FIELD_NAME_LENGTH];


std::string getDataLinkLayerType(pcpp::ProtocolType protocolType);
std::string getNetworkLayerType(pcpp::ProtocolType protocolType);
std::string getTransportLayerType(pcpp::ProtocolType protocolType);
std::string getApplicationLayerType(pcpp::ProtocolType protocolType);

class Layer
{
public:
    Layer(pcpp::Layer *layer){
        this->dataLen = layer->getHeaderLen();
        this->data = new uint8_t[this->dataLen];
        memcpy(this->data, layer->getData(), this->dataLen);
        this->type = layer->getProtocol();
        this->payloadLen = layer->getLayerPayloadSize();
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

    size_t getPayloadLen(){
        return this->payloadLen;
    }

    ~Layer(){
        delete[] this->data;
    }

    virtual void parse(pcpp::Packet &packet){}
    virtual void printLayer(){}

private:
    uint8_t* data;
    size_t dataLen;
    uint64_t type;
    size_t payloadLen;
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
            this->ackNumber = 0;
            this->sequenceNumber = 0;
            try
            {
                this->parse(packet);
            }
            catch(sniffer::WithoutLayer &e){
                std::cout << e.what() << std::endl;
            }
        }

        ~TransportLayer(){
            for(auto &option : this->tcpOptions){
                delete option;
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

        uint32_t getAckNumber(){
            return this->ackNumber;
        }

        uint32_t getSequenceNumber(){
            return this->sequenceNumber;
        }

        uint16_t getWindowSize(){
            return this->windowSize;
        }

        std::string getTcpFlags(){
            return this->tcpFlags;
        }

        int getTcpOptionsNum(){
            return this->tcpOptions.size();
        }

        static std::string printTcpOptionType(pcpp::TcpOptionType optionType);
        static std::string printTcpOptions(std::vector<pcpp::TcpOption*> tcpOptions);

        std::vector<pcpp::TcpOption*> getTcpOptions(){
            return this->tcpOptions;
        }

    private:
        std::string getTcpFlags(pcpp::TcpLayer* tcpLayer);
        uint16_t srcPort;
        uint16_t dstPort;
        uint32_t sequenceNumber;
        uint32_t ackNumber;
        uint16_t windowSize;
        std::string tcpFlags;
        std::vector<pcpp::TcpOption*> tcpOptions;
};

class ApplicationLayer : public Layer
{
    public:
        typedef struct
        {
            std::string method;
            std::string uri;
            std::string version;

            int stateCode;
            std::string stateMsg;

            std::string url;

            std::string fields[FIELD_NAME_LENGTH];
        }httpData_t;
        

        ApplicationLayer(pcpp::Layer *layer , pcpp::Packet &packet) : Layer(layer){
            try
            {
                this->parse(packet);
            }
            catch(sniffer::WithoutLayer &e){
                std::cout << e.what() << std::endl;
            }
        }

        ~ApplicationLayer(){

        }

        void parse(pcpp::Packet &packet) override;

        void printLayer() override;

    private:
        void* data = nullptr;
};



#endif