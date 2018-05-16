function(SOURCES_INCLUDE)
	include_directories(${CMAKE_SOURCE_DIR}/src)

endfunction()

function(SOURCES_TO_COMPILE PROJECT_NAME)
	add_executable(${PROJECT_NAME}

            ${CMAKE_SOURCE_DIR}/src/main.cpp
			${CMAKE_SOURCE_DIR}/src/events.cpp
			${CMAKE_SOURCE_DIR}/src/sdlinit.cpp
			${CMAKE_SOURCE_DIR}/src/gameinit.cpp
			${CMAKE_SOURCE_DIR}/src/keyboard.cpp


			${CMAKE_SOURCE_DIR}/src/ECS/Component/component.h
			${CMAKE_SOURCE_DIR}/src/ECS/Component/componentfactory.h
			${CMAKE_SOURCE_DIR}/src/ECS/Component/componentmanager.h
			${CMAKE_SOURCE_DIR}/src/ECS/Component/inputcomponent.h
			${CMAKE_SOURCE_DIR}/src/ECS/System/RenderingSystem.h


			${CMAKE_SOURCE_DIR}/src/entity.h
			${CMAKE_SOURCE_DIR}/src/EntityFactory.h
			${CMAKE_SOURCE_DIR}/src/entitymanager.h

			${CMAKE_SOURCE_DIR}/src/Event/event.h
			${CMAKE_SOURCE_DIR}/src/Event/eventmanager.h
			
			${CMAKE_SOURCE_DIR}/src/Event/eventgamepad.h
			${CMAKE_SOURCE_DIR}/src/Event/eventjoystick.h
			${CMAKE_SOURCE_DIR}/src/Event/eventkeyboard.h
			${CMAKE_SOURCE_DIR}/src/Event/eventkeyboard_sdlbridge.h
			${CMAKE_SOURCE_DIR}/src/Event/eventkeyboard_sdlbridge.cpp
			
			${CMAKE_SOURCE_DIR}/src/ShipFactory.h

            ${CMAKE_SOURCE_DIR}/src/Tech/G/Tech.h
			${CMAKE_SOURCE_DIR}/src/Tech/G/Tech.cpp
			
			${CMAKE_SOURCE_DIR}/src/Tech/SDL/Tech_SDLBridge.h
			${CMAKE_SOURCE_DIR}/src/Tech/SDL/Tech_SDLBridge.cpp

			${CMAKE_SOURCE_DIR}/src/Texture2D_SDLBridge.h

			${CMAKE_SOURCE_DIR}/src/SignalSlot/signalslot.h

			)
endfunction() 



function(LINK_STANDARD_LIBRARIES PROJECT_NAME)
	target_link_libraries(${ProjectName} "-liconv" "-lm")
endfunction()