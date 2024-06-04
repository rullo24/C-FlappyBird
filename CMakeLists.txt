cmake_minimum_required(VERSION 3.0)

# Project name
project(Flappy_Bird)

# Find SDL2 library
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Include directories for SDL headers
include_directories(${SDL2_INCLUDE_DIRS})

# Define the executable name (can be customized)
set(TARGET_NAME Flappy_Bird)

# Define compiler flags (excluding SDL flags)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

# Define source code files
set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")
set(SOURCE_FILES 
  ${SOURCE_DIR}/load_image.c 
  ${SOURCE_DIR}/misc.c 
  ${SOURCE_DIR}/game.c 
  ${SOURCE_DIR}/main.c
  )

# Link with SDL2 libraries
add_executable(${TARGET_NAME} ${SOURCE_FILES})
target_link_libraries(${TARGET_NAME} PUBLIC SDL2 SDL2::Image)
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
