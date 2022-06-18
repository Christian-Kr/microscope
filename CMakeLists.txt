cmake_minimum_required(VERSION 3.13.4)

project(microscope)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS "${PROJECT_SOURCE_DIR}/ui")

set(OpenCV_DIR "")
find_package(OpenCV REQUIRED)
find_package(Qt5 COMPONENTS Widgets SerialPort REQUIRED)

add_subdirectory(src)
