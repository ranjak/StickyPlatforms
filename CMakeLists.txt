project(stickyplatforms)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Configuration

# Hack to get library search paths right with MSVC
if (MSVC AND (CMAKE_SIZEOF_VOID_P EQUAL 8))
  set(CMAKE_LIBRARY_ARCHITECTURE x64)
else()
  set(CMAKE_LIBRARY_ARCHITECTURE x86)
endif()

set(STICKYPLATFORMS_STATIC OFF CACHE BOOL "Build StickyPlatforms statically")

# Dependencies

set(SDL2_STATIC ${STICKYPLATFORMS_STATIC})

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

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
    src/system/sdlinputhandler.cpp
    src/system/log.cpp
    src/system/display.cpp
    src/system/gamevector.cpp
    src/system/color.cpp
    src/system/rect.cpp
    src/system/util.cpp
    src/system/util_sdl.cpp
    src/system/resource.cpp
    src/system/mainloop.cpp
    src/system/graphics/texture.cpp
    src/system/graphics/image.cpp
    src/system/graphics/graphics.cpp
    src/system/graphics/rectangle.cpp
    src/system/graphics/frame.cpp
    src/system/graphics/text.cpp
    src/system/graphics/herosquare.cpp
    src/game.cpp
    src/gamestate.cpp
    src/loadlevelstate.cpp
    src/gameclearedstate.cpp
    src/playingstate.cpp
    src/pausedstate.cpp
    src/gamecommands.cpp
    src/world/level.cpp
    src/world/tile.cpp
    src/world/tileset.cpp
    src/world/tmxmaploader.cpp
    src/world/tsxtilesetloader.cpp
    src/test/test.cpp
    src/entities/entity.cpp
    src/entities/camera.cpp
    src/entities/cameracontroller.cpp
    src/entities/entityfactory.cpp
    src/entities/entitymanager.cpp
    src/entities/physicsmanager.cpp
    src/entities/triggertypes.cpp
    src/entities/states/actorstate.cpp
    src/entities/states/horizcontrolstate.cpp
    src/entities/states/groundstate.cpp
    src/entities/states/airstate.cpp
    src/entities/states/airclingablestate.cpp
    src/entities/states/jumpstate.cpp
    src/entities/states/clingstate.cpp
    src/entities/states/climbingstate.cpp
    src/entities/states/climbplatformstate.cpp
    src/entities/states/fallstate.cpp
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
    src/entities/components/graphicscomponent.cpp
    src/entities/components/cameracomponent.cpp
    src/entities/components/triggercomponent.cpp
    src/ui/uiwidget.cpp
    src/ui/healthbar.cpp
    src/ui/textwidget.cpp
    src/ui/uipanel.cpp
    src/ui/panelcontainer.cpp
)

set(HEADER_LIST
  src/system/make_unique.h
  src/entities/entitygroup.h
  src/entities/components/component.h
  src/entities/components/inputcomponent.h
  src/entities/components/message/message.h
  src/entities/components/message/directionchangedmsg.h
  src/entities/components/message/collision.h
  src/entities/components/message/childremovedmsg.h
  src/entities/components/message/damagemsg.h
  src/entities/components/message/triggermsg.h
  src/entities/components/message/cameracontrolmsg.h
  src/system/inputhandler.h
  src/system/sdlinputhandler.h
  src/system/log.h
  src/system/display.h
  src/system/gamevector.h
  src/system/color.h
  src/system/rect.h
  src/system/util.h
  src/system/util_sdl.h
  src/system/resource.h
  src/system/mainloop.h
  src/system/graphics/texture.h
  src/system/graphics/image.h
  src/system/graphics/graphics.h
  src/system/graphics/rectangle.h
  src/system/graphics/frame.h
  src/system/graphics/text.h
  src/system/graphics/herosquare.h
  src/game.h
  src/gamestate.h
  src/loadlevelstate.h
  src/gameclearedstate.h
  src/playingstate.h
  src/pausedstate.h
  src/gamecommands.h
  src/world/level.h
  src/world/tile.h
  src/world/tileset.h
  src/world/tmxmaploader.h
  src/world/tsxtilesetloader.h
  src/test/test.h
  src/entities/entity.h
  src/entities/camera.h
  src/entities/cameracontroller.h
  src/entities/entityfactory.h
  src/entities/entitymanager.h
  src/entities/physicsmanager.h
  src/entities/triggertypes.h
  src/entities/states/actorstate.h
  src/entities/states/horizcontrolstate.h
  src/entities/states/groundstate.h
  src/entities/states/airstate.h
  src/entities/states/airclingablestate.h
  src/entities/states/jumpstate.h
  src/entities/states/clingstate.h
  src/entities/states/climbingstate.h
  src/entities/states/climbplatformstate.h
  src/entities/states/fallstate.h
  src/entities/components/physicscomponent.h
  src/entities/components/staticphysicscomponent.h
  src/entities/components/movingphysicscomponent.h
  src/entities/components/playerinputcomponent.h
  src/entities/components/autoinputcomponent.h
  src/entities/components/basicaicomponent.h
  src/entities/components/actorcontrolcomponent.h
  src/entities/components/swordcomponent.h
  src/entities/components/weaponcomponent.h
  src/entities/components/healthcomponent.h
  src/entities/components/damagecomponent.h
  src/entities/components/graphicscomponent.h
  src/entities/components/cameracomponent.h
  src/entities/components/triggercomponent.h
  src/ui/uiwidget.h
  src/ui/healthbar.h
  src/ui/textwidget.h
  src/ui/uipanel.h
  src/ui/panelcontainer.h
)

add_subdirectory(src/3rdparty/tmxparser)

add_executable(stickyplatforms WIN32 ${SRC_LIST} ${HEADER_LIST} $<TARGET_OBJECTS:tmxparser>)

include_directories(
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/system
    ${CMAKE_CURRENT_SOURCE_DIR}/src/system/graphics
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
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
    ${SDL2_TTF_LIBRARIES}
)

# Build flags

set_property(TARGET stickyplatforms PROPERTY CXX_STANDARD 11)

if (NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter")
endif()

if (NOT CMAKE_BUILD_TYPE MATCHES [Dd]ebug)
    add_definitions(-DNDEBUG)
endif()

if (STICKYPLATFORMS_STATIC)
  target_link_libraries(${PROJECT_NAME} -Wl,-Bstatic -lharfbuzz -lfreetype -lpng -lbz2 -lz -lglib-2.0 -lpcre)
  # harfbuzz/freetype cyclic dependency
  target_link_libraries(${PROJECT_NAME} -Wl,-Bstatic -lharfbuzz)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -pthread -ldl")
endif()

# Installation

# Binaries
install(TARGETS stickyplatforms RUNTIME DESTINATION . CONFIGURATIONS Release)
install(TARGETS stickyplatforms RUNTIME DESTINATION . CONFIGURATIONS Debug RENAME stickyplatforms-debug)
# Resources
install(DIRECTORY assets/ DESTINATION assets)
