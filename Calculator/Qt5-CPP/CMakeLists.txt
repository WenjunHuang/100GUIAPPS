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
find_package(GoogleTest REQUIRED)

add_subdirectory(number)

add_executable(Calculator main.cpp
        Button.h
        Button.cc
        Calculator.cc
        Calculator.h)
target_link_libraries(Calculator LINK_PRIVATE Qt5::Widgets
        RxCpp
        GoogleTest
        number
        )
target_include_directories(Calculator PUBLIC  ${GMP_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/knumber)
