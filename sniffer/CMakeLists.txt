cmake_minimum_required(VERSION 3.13)

project(Sniffer)

include_directories(include third_party/PcapPlusPlus/include
                    third_party/SQLiteCpp/include)

set(SOURCES src/main.cpp
            src/sniffer.cpp
            src/datamanager.cpp)

set(PcapPlusPlus_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/PcapPlusPlus/lib/cmake/pcapplusplus)
set(SQLiteCpp_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/SQLiteCpp/lib/cmake/SQLiteCpp)

find_package(PcapPlusPlus REQUIRED)
find_package(SQLiteCpp REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC PcapPlusPlus::Pcap++
                                            SQLiteCpp)