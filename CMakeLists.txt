cmake_minimum_required(VERSION 3.9.0)
message(STATUS "===========================================================")
set(QTPTH $ENV{HOME}/Qt510)
message(STATUS "QT PATH: ${QTPTH}")

project(XMLReaderSample)
message("Hola mundo!!!")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Xml REQUIRED)

add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Xml)

include_directories("${QTPTH}/5.10.0/clang_64/lib/QtXml.framework/Headers/")

#In http://doc.qt.io/qt-5/cmake-manual.html
#Read about:
# Imported targets and their locations
# Variable Reference and Module Variables Section (This clarifies everything)



