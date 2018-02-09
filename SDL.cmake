function(SDL_INCLUSION)
	include_directories(${CMAKE_SOURCE_DIR}/3rd/include)
endfunction()


function(SDL_LINK PROJECT_NAME)
	target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/3rd/lib/libSDL2.a)
endfunction()



