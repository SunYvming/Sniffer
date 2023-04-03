cmake_minimum_required(VERSION 3.13)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

project(Sniffer)

include_directories(form
                    include
                    third_party/PcapPlusPlus/include
                    third_party/SQLiteCpp/include
                    third_party/boost/include)

set(CMAKE_AUTOUIC_SEARCH_PATHS form)

set(SOURCES src/main.cpp
            src/sniffer.cpp
            src/datamanager.cpp
            src/dbloader.cpp
            src/layer.cpp
            src/mainwindow.cpp
            form/mainwindow.ui
            src/infowidget.cpp
            form/infowidget.ui
            src/infocard.cpp
            form/infocard.ui
            src/analyzewidget.cpp
            form/analyzewidget.ui
            src/logframe.cpp
            form/logframe.ui
            src/layercard.cpp
            form/layercard.ui
            include/datamanager.hpp
            include/dbloader.hpp
            include/layer.hpp
            include/sniffer.hpp
            include/exception.hpp
            include/mainwindow.h
            include/infowidget.h
            include/infocard.h
            include/analyzewidget.h
            include/logframe.h
            include/layercard.h
            )

set(PcapPlusPlus_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/PcapPlusPlus/lib/cmake/pcapplusplus)
set(SQLiteCpp_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/SQLiteCpp/lib/cmake/SQLiteCpp)
set(Boost_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost/lib/cmake/Boost-1.81.0)
set(Boost_NO_SYSTEM_PATHS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")

find_package(PcapPlusPlus REQUIRED)
find_package(SQLiteCpp REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package(Qt6 COMPONENTS Widgets REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC PcapPlusPlus::Pcap++
                                            SQLiteCpp
                                            ${Boost_LIBRARIES}
                                            Qt6::Widgets
                                            )