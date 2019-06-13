cmake_minimum_required(VERSION 3.13)
project(Calculator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(CMAKE_PREFIX_PATH $ENV{QT_CMAKE})
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(GMP)
find_package(Qt5 REQUIRED Core Widgets)
find_package(RxCpp REQUIRED)


add_executable(Calculator main.cpp
        Button.h
        Button.cc
        Calculator.cc
        Calculator.h
        number/Number.cc number/NumberBase.h number/NumberInteger.cc number/NumberInteger.h)
target_link_libraries(Calculator LINK_PRIVATE Qt5::Widgets ${GMP_LIBRARIES})
target_include_directories(Calculator PUBLIC ${rxcpp_INCLUDE_DIR} ${GMP_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/knumber)
