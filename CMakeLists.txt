cmake_minimum_required(VERSION 3.10)
project(Gravity)

set(CMAKE_CXX_STANDARD 17)

# Trova SDL2
find_package(SDL2 REQUIRED)

# Includi le directory di SDL2 (per gli header)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(gravity
    src/main.cpp
    src/utilities/utils.cpp
)

# Linka SDL2 al tuo eseguibile
target_link_libraries(gravity ${SDL2_LIBRARIES})