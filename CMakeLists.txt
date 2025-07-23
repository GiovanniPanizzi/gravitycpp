cmake_minimum_required(VERSION 3.10)
project(Gravity)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(OpenGL REQUIRED)

include_directories(
    ${SDL2_INCLUDE_DIRS}
    thirdParts/glad/include
    include
)

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

    thirdParts/glad/src/gl.c 
)

target_link_libraries(gravity
    ${SDL2_LIBRARIES}
    OpenGL::GL
)

target_compile_options(gravity PRIVATE -Wall -Wextra -Wpedantic)

