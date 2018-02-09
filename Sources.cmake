function(SOURCES_INCLUDE)
	include_directories(${CMAKE_SOURCE_DIR}/src)
endfunction()

function(SOURCES_TO_COMPILE PROJECT_NAME)
	add_executable(${PROJECT_NAME} 
		${CMAKE_SOURCE_DIR}/src/main.cpp)
endfunction() 



function(LINK_STANDARD_LIBRARIES PROJECT_NAME)
	target_link_libraries(${ProjectName} "-liconv" "-lm")
endfunction()