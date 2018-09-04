# Space Invaders Clone

This is an example to illustrate Entity_ Component System.

## Release Notes

 Version| Description
--------|------------
 0.0.8  | Bolt LifeSpan (Time)
 0.0.7  | Background moves as ship thrust up with Up Arrow Key
 0.0.6  | ???
 0.0.5  | ???
 0.0.4  | ???
 0.0.3  | ???
 0.0.2  | LoadTexture, CreateTextureWithSize and DestroyTexture
 0.0.1  | First Sample for a Rectangle Texture Rendered
 0.0.0  | Starting version

## Not official Installation Instruction (Thanks José de Paz)

### Prerequisites

- Following steps are made using **cmake 3.10**
- Check versions and availiability. Install if needed
  - `g++ --version`
  - `cmake --version`

### Steps

0. Install dependent libraries:
   - **libpng-dev** (or **libpng-devel** in OpenSuse)
   - **xorg-dev** (or **xorg-x11-devel** in OpenSuse)
   - **libasound2-dev** (or **alsa-dev** in OpenSuse)
   - **pulseaudio**
   - **libs for SDL2** (_look at the result of the `./configure` command_)
     - libsamplerate
     - libudev
     - dbus
     - ime
     - ibus
     - fcitx

1. Download **SDL, SDL_IMAGE** and **GLM** packages:
   - SDL2-2.0.8.zip
   - SDL2_image-2.0.3.zip
   - glm-0.9.7.5.zip

1. Compile and install **SDL2-2.0.8**:
   - Decompress SDL2-2.0.8.zip
   - Enter the "SDL2-2.0.8" folder
   - Run `./configure`   (***)
   - Run `make`
   - Run `make install` (_with sudo privileges_)

1. Compile and install **SDL2_image-2.0.3**:
   - Decompress SDL2_image-2.0.3.zip
   - Enter the "SDL2_image-2.0.3" folder
   - Run `./configure`
   - Run `make`
   - Run `make install` (_with sudo privileges_)

1. Compile and install **glm-0.9.7.6**:
   - Decompress glm-0.9.7.6.zip
   - Create folder "build"
   - Enter the "build" folder
   - Run `cmake ..`
   - Run `make`
   - Run `make install` (_with sudo privileges_)

1. Modify **martin-the-martian** files:
   - Open the **CMakeLists.txt** file and modify:
     - Comment the line (_prefixing it with #_):
       ```cmake
       include(cmake/SDL.cmake)
       ```
     - Comment the line:
       ```cmake
       include(cmake/MacOS.cmake)
       ```
     - Comment the next lines:
       ```cmake
       PrintLine()
       HeaderMessage("SDL2 Libraries Linkage Addition")
       SDL_LINK(${ProjectName})

       PrintLine()
       HeaderMessage("Cocoa Frameworks Linkage Addition")
       COCOA_LINK(${ProjectName})
       ```
     - save the file.
     
   - Open the **cmake/Sources.cmake** file and modify:
     - modify the line:
       ```cmake
       target_link_libraries(${ProjectName} "-liconv" "-lm")
       ```
       with this value:
       ```cmake
       target_link_libraries(${ProjectName} "-lSDL2" "-lSDL2_image" "-lm")
       ```
     - save the file.

   - Open the **src/sdlwrapper.cpp** file and modify:
     - modify the line:
       ```cpp
       #include <SDL2_image/SDL_image.h>
       ```
       with this value:
       ```cpp
       #include <SDL2/SDL_image.h>
       ```
     - save the file.

   - Open the **src/Sprite.h** file and modify:
     - modify the line:
       ```cpp
       #include <SDL2_image/SDL_image.h>
       ```
       with this value:
       ```cpp
       #include <SDL2/SDL_image.h>
       ```
     - save the file.
1. Compile **martin-the-martian** project:
   - Create folder "build"
   - Enter the "build" folder
   - Run `cmake ..`
   - Run `make`
   - Run `./VoidSample`
