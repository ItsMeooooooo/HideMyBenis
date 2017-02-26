# HideMyBenis
Steganographic software that hides files in images.

## Build info
You need to install the sodium library (https://download.libsodium.org/doc/).  
The rudimentary FindSodium.cmake expects either:  
* the CMake variable SODIUM_ROOT set to the root of the library
* or alternatively, the system environment variable SODIUM_ROOT set accordingly

You will also need:  
* stb_image: https://github.com/nothings/stb/blob/master/stb_image.h
* stb_image_write: https://github.com/nothings/stb/blob/master/stb_image_write.h  
You should set STB_INCLUDE_DIR accordingly (or put them into project root).

If you also want to compile the GUI executable, you will need Qt5.
