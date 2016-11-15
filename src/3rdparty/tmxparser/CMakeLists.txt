# TMXParser / TSXParser CMake file for StickyPlatforms

set(SRC_LIST
    src/TMXParser.cpp
    src/TSXParser.cpp
    src/tmxcommon.cpp
    src/common.cpp
)

include_directories(include src)

# CMake < 3.1 doesn't support object libraries

if (CMAKE_VERSION VERSION_LESS 3.1)
  add_library(tmxparser STATIC ${SRC_LIST})
else()
  add_library(tmxparser OBJECT ${SRC_LIST})
endif()
