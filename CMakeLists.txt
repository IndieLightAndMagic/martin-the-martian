#SDL2 Example CMakeLists.txt
#
#   Author: Julián Andrés Guarín Reyes
#   Version: 0.0.1
#   Date: Feb, 2018.

#Utility messages
cmake_minimum_required(VERSION 3.9.0)
include(Messages.cmake)

set(CMAKE_VERBOSE_MAKEFILE ON)

PrintLine()
HeaderMessage("SDL Template Example By Julian Guarin")
PrintLine()

#Info variables
include(Info.cmake)

#Sources to include
include(Sources.cmake)

#Resources
include(Resources.cmake)

#SDL2 
include(SDL.cmake)

#Cocoa 
include(MacOS.cmake)

#Set C++
include(CPP.cmake)
USE_14()

#Project is SDL VoidSample
set(ProjectName VoidSample)
project(${ProjectName})

PrintLine()
HeaderMessage("Adding Headers To Project")
SOURCES_INCLUDE()

PrintLine()
HeaderMessage("Adding Sources To Project")
SOURCES_TO_COMPILE(${ProjectName})

PrintLine()
HeaderMessage("SDL2 Libraries Linkage Addition")
SDL_LINK(${ProjectName})

PrintLine()
HeaderMessage("Cocoa Frameworks Linkage Addition")
COCOA_LINK(${ProjectName})

PrintLine()
HeaderMessage("Standard Linkage Addition")
LINK_STANDARD_LIBRARIES(${ProjectName})
