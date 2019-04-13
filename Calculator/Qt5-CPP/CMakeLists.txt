cmake_minimum_required(VERSION 3.13)
project(Calculator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(CMAKE_PREFIX_PATH $ENV{QT_CMAKE})

find_package(Qt5 REQUIRED Core Widgets)

add_executable(Calculator main.cpp Button.h Button.cc Calculator.cc Calculator.h)
qt5_use_modules(Calculator LINK_PRIVATE Widgets)