project(game1)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Dependencies

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

# Project setup

# Check for make_unique

set(CMAKE_REQUIRED_FLAGS "-std=c++11")
include(CheckCXXSourceCompiles)

file(READ config/make_unique_test.cpp MAKE_UNIQUE_TEST_SRC)
check_cxx_source_compiles("${MAKE_UNIQUE_TEST_SRC}" HAVE_MAKE_UNIQUE)
configure_file(config/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/config.h)


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
    src/system/mainloop.cpp
    src/gamestate.cpp
    src/gamecommands.cpp
    src/graphics.cpp
    src/rectangle.cpp
    src/world/level.cpp
    src/world/tile.cpp
    src/world/tileset.cpp
    src/world/tmxmaploader.cpp
    src/world/tsxtilesetloader.cpp
    src/test/test.cpp
    src/entities/entity.cpp
    src/entities/camera.cpp
    src/entities/entityfactory.cpp
    src/entities/entitymanager.cpp
    src/entities/physicsmanager.cpp
    src/entities/states/actorstate.cpp
    src/entities/states/horizcontrolstate.cpp
    src/entities/states/groundstate.cpp
    src/entities/states/airstate.cpp
    src/entities/states/airclingablestate.cpp
    src/entities/states/jumpstate.cpp
    src/entities/states/clingstate.cpp
    src/entities/states/climbingstate.cpp
    src/entities/states/climbplatformstate.cpp
    src/entities/components/physicscomponent.cpp
    src/entities/components/staticphysicscomponent.cpp
    src/entities/components/movingphysicscomponent.cpp
    src/entities/components/playerinputcomponent.cpp
    src/entities/components/autoinputcomponent.cpp
    src/entities/components/basicaicomponent.cpp
    src/entities/components/actorcontrolcomponent.cpp
    src/entities/components/swordcomponent.cpp
    src/entities/components/weaponcomponent.cpp
    src/entities/components/healthcomponent.cpp
    src/entities/components/damagecomponent.cpp
)

# Header-only sources, explicitly added so that IDEs can see them
set(HEADER_LIST
  src/system/make_unique.h
  src/entities/entitygroup.h
  src/entities/components/component.h
  src/entities/components/inputcomponent.h
  src/entities/components/message/message.h
  src/entities/components/message/directionchangedmsg.h
  src/entities/components/message/collisionmsg.h
  src/entities/components/message/childremovedmsg.h
)

add_subdirectory(src/3rdparty/tmxparser)

add_executable(game1 WIN32 ${SRC_LIST} ${HEADER_LIST} $<TARGET_OBJECTS:tmxparser>)

include_directories(
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/system
    ${CMAKE_CURRENT_SOURCE_DIR}/src/entities
    ${CMAKE_CURRENT_SOURCE_DIR}/src/entities/states
    ${CMAKE_CURRENT_SOURCE_DIR}/src/entities/components
    ${CMAKE_CURRENT_SOURCE_DIR}/src/entities/components/message
    ${CMAKE_CURRENT_SOURCE_DIR}/src/3rdparty/tmxparser/include
	${CMAKE_CURRENT_BINARY_DIR}/include
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
