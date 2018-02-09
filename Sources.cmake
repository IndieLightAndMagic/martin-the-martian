

#Sources 
set(SOURCES_CPP 
	${CMAKE_SOURCE_DIR}/main.cpp
	)


#Headers
set(SOURCES_HEADERS ${CMAKE_SOURCE_DIR}/src)




function(SOURCES_INCLUDE)
	include_directories(${SOURCES_HEADERS})
endfunction()

function(SOURCES_TO_COMPILE PROJECT_NAME)
	add_executable(${PROJECT_NAME} ${SOURCES_CPP})
endfunction() 
