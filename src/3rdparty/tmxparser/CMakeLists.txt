# TMXParser / TSXParser CMake file for game1

set(SRC_LIST
    src/TMXParser.cpp
    src/TSXParser.cpp
)

include_directories(include)

add_library(tmxparser OBJECT ${SRC_LIST})
