cmake_minimum_required(VERSION 3.13)

project(Snipper)

include_directories(include)

set(SOURCES src/main.cpp)

include(FindPCAP.cmake)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${PCAP_LIBRARY})