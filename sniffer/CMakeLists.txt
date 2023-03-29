cmake_minimum_required(VERSION 3.13)

project(Sniffer)

include_directories(include third_party/PcapPlusPlus/include)

set(SOURCES src/main.cpp
            src/sniffer.cpp
            src/datamanager.cpp)

set(PcapPlusPlus_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/PcapPlusPlus/lib/cmake/pcapplusplus)

find_package(PcapPlusPlus REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC PcapPlusPlus::Pcap++)