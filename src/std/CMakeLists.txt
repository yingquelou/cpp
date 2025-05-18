cmake_path(GET CMAKE_CURRENT_LIST_DIR FILENAME PROJ)
set(CMAKE_CXX_STANDARD 11)
add_executable(${PROJ} main.cpp)