g++ -std=c++11 -o gravity main.cpp globals.cpp \
classes/Draw.cpp classes/EventListener.cpp classes/Galaxy.cpp classes/Phisics.cpp \
classes/EntityController.cpp classes/Collider.cpp classes/GalaxyEdit.cpp \
-I/opt/homebrew/opt/sdl2/include/SDL2 \
-I/opt/homebrew/opt/sdl2_image/include/SDL2 \
-L/opt/homebrew/opt/sdl2/lib \
-L/opt/homebrew/opt/sdl2_image/lib \
-lSDL2 -lSDL2_image