function(SDL_INCLUSION)
	set(SDL_HEADERS ${SDL_PROJECTS_DIR}/${3rd}/include)
	set(SDL_LIBS_TO_LINK ${SDL_PROJECTS_DIR}/${3rd}/libSDL2d.a)
endfunction()


function(SDL_LINK PROJECT_NAME)
	target_link_libraries(${PROJECT_NAME} ${SDL_LIBS_TO_LINK})
endfunction()



