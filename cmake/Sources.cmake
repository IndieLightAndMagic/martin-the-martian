function(SOURCES_INCLUDE)
    include_directories(${CMAKE_SOURCE_DIR}/src)

endfunction()

function(SOURCES_TO_COMPILE PROJECT_NAME)
    add_executable(${PROJECT_NAME}

            ${CMAKE_SOURCE_DIR}/src/main.cpp
            ${CMAKE_SOURCE_DIR}/src/sdlwrapper.cpp
            ${CMAKE_SOURCE_DIR}/src/game.cpp
            ${CMAKE_SOURCE_DIR}/src/keyboard.cpp

            ${CMAKE_SOURCE_DIR}/src/timermanager.cpp
            ${CMAKE_SOURCE_DIR}/src/timermanager.h
            ${CMAKE_SOURCE_DIR}/src/timer.cpp
            ${CMAKE_SOURCE_DIR}/src/timer.h

            ${CMAKE_SOURCE_DIR}/src/ECS/Component/lifespancomponent.cpp
            ${CMAKE_SOURCE_DIR}/src/ECS/Component/anchorpointcomponent.cpp
            ${CMAKE_SOURCE_DIR}/src/ECS/Component/texturecomponent.cpp
            ${CMAKE_SOURCE_DIR}/src/ECS/Component/entityinformationcomponent.cpp
            ${CMAKE_SOURCE_DIR}/src/ECS/Component/componentfactory.cpp
            ${CMAKE_SOURCE_DIR}/src/ECS/Component/componentmanager.cpp
            ${CMAKE_SOURCE_DIR}/src/ECS/Component/component.h

            ${CMAKE_SOURCE_DIR}/src/ECS/System/rendering.cpp
            ${CMAKE_SOURCE_DIR}/src/ECS/System/kinematics.cpp

            ${CMAKE_SOURCE_DIR}/src/ECS/Event/events.cpp
            ${CMAKE_SOURCE_DIR}/src/ECS/Event/events.h

            ${CMAKE_SOURCE_DIR}/src/ECS/Entity/entityfactory.cpp
            ${CMAKE_SOURCE_DIR}/src/ECS/Entity/entitymanager.cpp
            ${CMAKE_SOURCE_DIR}/src/ECS/Entity/entity.h


            ${CMAKE_SOURCE_DIR}/src/Event/event.h
            ${CMAKE_SOURCE_DIR}/src/Event/eventmanager.h

            ${CMAKE_SOURCE_DIR}/src/Event/eventgamepad.h
            ${CMAKE_SOURCE_DIR}/src/Event/eventjoystick.h
            ${CMAKE_SOURCE_DIR}/src/Event/eventkeyboard.h
            ${CMAKE_SOURCE_DIR}/src/Event/eventkeyboard_sdlbridge.h

            ${CMAKE_SOURCE_DIR}/src/Sprite.cpp

            ${CMAKE_SOURCE_DIR}/src/SignalSlot/signalslot.h

            )
endfunction()



function(LINK_STANDARD_LIBRARIES PROJECT_NAME)
    target_link_libraries(${ProjectName} "-lSDL2" "-lSDL2_image" "-lm")
endfunction()
