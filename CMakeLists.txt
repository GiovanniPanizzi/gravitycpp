cmake_minimum_required(VERSION 3.10)
project(Gravity)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Trova SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Trova OpenGL
find_package(OpenGL REQUIRED)

# Includi le directory header
include_directories(
    ${SDL2_INCLUDE_DIRS}
    thirdParts/glad/include
    include
)

# Aggiungi eseguibile con i sorgenti, includendo anche glad/src/gl.c
add_executable(gravity
    src/main.cpp

    src/classes/generalPurpose/Window.cpp
    src/classes/generalPurpose/EventListener.cpp
    src/classes/generalPurpose/Draw.cpp

    src/classes/systems/Phisics.cpp 
    src/classes/systems/Collider.cpp
    src/classes/systems/HumanController.cpp

    src/objects/Galaxy.cpp

    src/utilities/utils.cpp
    src/utilities/globals.cpp

    thirdParts/glad/src/gl.c      # qui includi il sorgente glad
)

# Linka le librerie necessarie
target_link_libraries(gravity
    ${SDL2_LIBRARIES}
    OpenGL::GL
)

# Flag per warning
target_compile_options(gravity PRIVATE -Wall -Wextra -Wpedantic)

