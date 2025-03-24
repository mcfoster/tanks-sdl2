# // Copyright (c) 2025 Martin Chris Foster
# // This file is part of the tanks_sdl2 project.
# // tanks_sdl2 is free software: you can redistribute it and/or modify
# // it under the terms of the GNU General Public License as published by
# // the Free Software Foundation, either version 3 of the License, or
# // (at your option) any later version.
# // tanks_sdl2 is distributed in the hope that it will be useful,
# // All rights reserved.

cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 17)
project(tanks_sdl2)
find_package(SDL2 REQUIRED)
#find_package(SDL2TTF REQUIRED)
#find_package(SDL2_mixer REQUIRED)

#find_package(SDL2_image REQUIRED)
#include_directories(${PROJECT_NAME} ${SDL2_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS})
include_directories(${PROJECT_NAME} ${SDL2_INCLUDE_DIRS})

add_executable(${PROJECT_NAME}
        main.cpp
        DrawText.cpp
        gameMessageBox.cpp
)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} SDL2_ttf SDL2_image SDL2_mixer)
#target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_MIXER_LIBRARIES} SDL2_ttf SDL2_image)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/images
        ${CMAKE_BINARY_DIR}/images)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/fonts
        ${CMAKE_BINARY_DIR}/fonts)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/sounds
        ${CMAKE_BINARY_DIR}/sounds)
