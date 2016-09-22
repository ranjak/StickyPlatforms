# TMXParser / TSXParser CMake file for game1

set(SRC_LIST
    src/TMXParser.cpp
    src/TSXParser.cpp
    src/common.cpp
)

include_directories(include src)

add_library(tmxparser OBJECT ${SRC_LIST})
