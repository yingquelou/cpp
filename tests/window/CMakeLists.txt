cmake_path(GET CMAKE_CURRENT_LIST_DIR FILENAME PROJ)
if(WIN32)
    add_executable(${PROJ} WIN32 main.c ok.rc)
endif()
