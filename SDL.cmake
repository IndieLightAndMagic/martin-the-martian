add_definitions(-DPNG_FORMAT_SHOULD_BE_USED)

function(SDL_LINK PROJECT_NAME)
	target_link_libraries(${PROJECT_NAME}
		${CMAKE_SOURCE_DIR}/3rd/lib/SDL2_image.framework/SDL2_image	
		${CMAKE_SOURCE_DIR}/3rd/lib/SDL2.framework/SDL2)	
endfunction()




