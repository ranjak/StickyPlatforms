project(game1)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Dependencies

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

# Project setup

set(SRC_LIST
    src/system/main.cpp
    src/system/inputhandler.cpp
    src/system/log.cpp
    src/system/display.cpp
    src/system/gamevector.cpp
    src/system/color.cpp
    src/system/rect.cpp
    src/system/util.cpp
    src/system/util_sdl.cpp
    src/system/image.cpp
    src/system/resource.cpp
    src/gamestate.cpp
    src/gamecommands.cpp
    src/entity.cpp
    src/hero.cpp
    src/graphics.cpp
    src/rectangle.cpp
    src/herostate.cpp
    src/horizcontrolstate.cpp
    src/groundstate.cpp
    src/airstate.cpp
    src/world/level.cpp
    src/world/tile.cpp
    src/test/test.cpp
    src/camera.cpp
    src/enemy.cpp
    src/swordstate.cpp
    src/movement.cpp
)

add_executable(game1 ${SRC_LIST})

include_directories(
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/system
)
target_link_libraries(${PROJECT_NAME}
    ${SDL2_LIBRARY}
    ${SDL2_IMAGE_LIBRARIES}
)

# Build flags

set_property(TARGET game1 PROPERTY CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Wall)

if (NOT CMAKE_BUILD_TYPE MATCHES [Dd]ebug)
    add_definitions(-DNDEBUG)
endif()
