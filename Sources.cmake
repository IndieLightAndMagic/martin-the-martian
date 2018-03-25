function(SOURCES_INCLUDE)
	include_directories(${CMAKE_SOURCE_DIR}/src)

endfunction()

function(SOURCES_TO_COMPILE PROJECT_NAME)
	add_executable(${PROJECT_NAME}

            ${CMAKE_SOURCE_DIR}/src/main.cpp
            ${CMAKE_SOURCE_DIR}/src/Tech.cpp

			${CMAKE_SOURCE_DIR}/src/component.h
			${CMAKE_SOURCE_DIR}/src/componentfactory.h
			${CMAKE_SOURCE_DIR}/src/componentmanager.h

			${CMAKE_SOURCE_DIR}/src/entity.h
			${CMAKE_SOURCE_DIR}/src/EntityFactory.h
			${CMAKE_SOURCE_DIR}/src/entitymanager.h

			${CMAKE_SOURCE_DIR}/src/ShipFactory.h

			${CMAKE_SOURCE_DIR}/src/Tech_SDLBridge.h

			${CMAKE_SOURCE_DIR}/src/Texture2D_SDLBridge.h

			${CMAKE_SOURCE_DIR}/src/SignalSlot/signalslot.h

			)
endfunction() 



function(LINK_STANDARD_LIBRARIES PROJECT_NAME)
	target_link_libraries(${ProjectName} "-liconv" "-lm")
endfunction()