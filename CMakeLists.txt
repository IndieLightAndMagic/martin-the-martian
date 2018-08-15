#SDL2 Example CMakeLists.txt
#
#   Author: Julián Andrés Guarín Reyes
#   Version: 0.0.1
#   Date: Feb, 2018.

#Utility messages
cmake_minimum_required(VERSION 3.5.1)
include(cmake/Messages.cmake)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(ProjectName VoidSample)
project(${ProjectName})


PrintLine()
HeaderMessage("SDL Template Example By Julian Guarin")
PrintLine()

#Info variables
include(cmake/Info.cmake)

#Sources to include
include(cmake/Sources.cmake)

#Resources
include(cmake/Resources.cmake)

if (APPLE)
#SDL2 
include(cmake/SDL.cmake)
endif()

#GML 
include(cmake/GLM.cmake)

if (APPLE)
#Cocoa 
include(cmake/MacOS.cmake)
endif()


#Set C++
include(cmake/CPP.cmake)
#USE_14()
TRY_17()


if ("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
PrintLine()
message(STATUS "-> Linux OS <-")
endif()

#Project is SDL VoidSample

PrintLine()
HeaderMessage("Adding Headers To Project")
SOURCES_INCLUDE()
GLM_INCLUDE()

PrintLine()
HeaderMessage("Adding Sources To Project")
SOURCES_TO_COMPILE(${ProjectName})

if (APPLE)
PrintLine()
HeaderMessage("SDL2 Libraries Linkage Addition")
SDL_LINK(${ProjectName})

PrintLine()
HeaderMessage("Cocoa Frameworks Linkage Addition")
COCOA_LINK(${ProjectName})
endif()

PrintLine()
HeaderMessage("Standard Linkage Addition")
LINK_STANDARD_LIBRARIES(${ProjectName})
