﻿cmake_path(GET CMAKE_CURRENT_LIST_DIR FILENAME PROJ)
if(WIN32)
    add_executable(${PROJ} coTest.c)
endif()
