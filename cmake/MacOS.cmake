function(COCOA_LINK PROJECT_NAME)
	target_link_libraries(${PROJECT_NAME}
		"-framework Carbon" 
		"-framework ForceFeedback" 
		"-framework IOKit" 
		"-framework Cocoa" 
		"-framework CoreVideo"
		"-framework AudioToolbox"
		"-framework CoreAudio"
		"-framework Foundation"
		"-framework AppKit"
		"-framework GLUT"
		"-framework OpenGL")
endfunction()