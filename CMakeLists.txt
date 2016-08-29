project(game1)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Dependencies

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Project setup

set(SRC_LIST
    src/main.cpp
    src/inputhandler.cpp
    src/log.cpp
    src/display.cpp
    src/gamestate.cpp
    src/gamecommands.cpp
    src/entity.cpp
    src/hero.cpp
    src/graphics.cpp
    src/playercube.cpp
)

add_executable(${PROJECT_NAME} ${SRC_LIST})

include_directories(
    ${SDL2_INCLUDE_DIR}
    ${SDL2_TTF_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(${PROJECT_NAME}
    ${SDL2_LIBRARY}
    ${SDL2_TTF_LIBRARIES}
)

# Build flags
if (NOT CMAKE_BUILD_TYPE MATCHES [Dd]ebug)
    add_definitions(-DNDEBUG)
endif()