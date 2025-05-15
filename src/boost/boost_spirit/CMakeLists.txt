cmake_path(GET CMAKE_CURRENT_LIST_DIR FILENAME PROJ)
add_executable(${PROJ} boost_spirit.cpp)