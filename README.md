Space Invaders Clone

This is an example to illustrate Entity_ Component System.


# Release Notes

Version -- Description


0.0.8	-- Bolt LifeSpan (Time).

0.0.7	-- Background moves as ship thrust up with Up Arrow Key

0.0.6	-- ???

0.0.5	-- ???

0.0.4	-- ???

0.0.3	-- ???

0.0.2	-- LoadTexture, CreateTextureWithSize and DestroyTexture.

0.0.1	-- First Sample for a Rectangle Texture Rendered.

0.0.0   -- Starting version.


# Not official Installation Instruction (Thanks José de Paz)

- Compiling using g++ 7 and cmake 3.10

0. Install dependent libraries:
   
   libpng-dev (or libpng-devel in OpenSuse )
   xorg-dev (or xorg-x11-devel in OpenSuse )
   libasound2-dev (or alsa-dev in OpenSuse )
   pulseaudio
   
   (*** libs for SDL2 -- look at the result of the "./configure" command --)
   libsamplerate
   libudev
   dbus
   ime 
   ibus 
   fcitx 


1. Download SDL, SDL_IMAGE and GLM packages:

   SDL2-2.0.8.zip      
   SDL2_image-2.0.3.zip   
   glm-0.9.7.5.zip

2. Compile and install SDL2-2.0.8:
   - decompress SDL2-2.0.8.zip
   - enter the "SDL2-2.0.8" folder
   - run "./configure"   (***)
   - run "make" 
   - run "make install" (with sudo privileges)

3. Compile and install SDL2_image-2.0.3:
   - decompress SDL2_image-2.0.3.zip
   - enter the "SDL2_image-2.0.3" folder
   - run "./configure"
   - run make
   - run make install (with sudo privileges)

4. Compile and install glm-0.9.7.6:
   - decompress glm-0.9.7.6.zip
   - create folder "build"
   - enter the "build" folder
   - run "cmake .."   
   - run "make"
   - run "make install" (with sudo privileges)

5. Modify "martin-the-martian" files:

   - Open the "CMakeLists.txt" file and modify:
    - comment the line "include(cmake/SDL.cmake)" 
    - comment the line "include(cmake/MacOS.cmake)"
    - comment the next lines:
       PrintLine()
       HeaderMessage("SDL2 Libraries Linkage Addition")
       SDL_LINK(${ProjectName})

       PrintLine()
       HeaderMessage("Cocoa Frameworks Linkage Addition")
       COCOA_LINK(${ProjectName})
     - save the file.

   - Open the "cmake/Sources.cmake" file and modify:
    - modify the line: 
       target_link_libraries(${ProjectName} "-liconv" "-lm")

      with this values:
       
       target_link_libraries(${ProjectName} "-lSDL2" "-lSDL2_image" "-lm")
    - save the file.

   - Open the "src/sdlwrapper.cpp" file and modify:
    - modify the line: 
       #include <SDL2_image/SDL_image.h>

      with this values:
       #include <SDL2/SDL_image.h>
    - save the file.
    
   - Open the "src/Sprite.h" file and modify:
    - modify the line: 
       #include <SDL2_image/SDL_image.h>

      with this values:
       #include <SDL2/SDL_image.h>
    - save the file.
    
6. Compile "martin-the-martian" project:
   
   - create folder "build"
   - enter the "build" folder
   - run "cmake .."   
   - run "make"
   - run "./VoidSample
