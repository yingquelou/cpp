cmake_path(GET CMAKE_CURRENT_LIST_DIR FILENAME PROJ)
if(WIN32)
    add_executable(${PROJ} WIN32 main.cpp window_wrapper.cpp)
endif()
