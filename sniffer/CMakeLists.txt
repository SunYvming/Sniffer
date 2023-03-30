cmake_minimum_required(VERSION 3.13)

project(Sniffer)

include_directories(include third_party/PcapPlusPlus/include
                    third_party/SQLiteCpp/include
                    third_party/boost/include)

set(SOURCES src/main.cpp
            src/sniffer.cpp
            src/datamanager.cpp
            src/layer.cpp)

set(PcapPlusPlus_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/PcapPlusPlus/lib/cmake/pcapplusplus)
set(SQLiteCpp_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/SQLiteCpp/lib/cmake/SQLiteCpp)
set(Boost_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost/lib/cmake/Boost-1.81.0)
set(Boost_NO_SYSTEM_PATHS ON)

find_package(PcapPlusPlus REQUIRED)
find_package(SQLiteCpp REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC PcapPlusPlus::Pcap++
                                            SQLiteCpp
                                            ${Boost_LIBRARIES}
                                            )