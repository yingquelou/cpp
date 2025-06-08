cmake_path(GET CMAKE_CURRENT_LIST_DIR FILENAME PROJ)
if(MSVC AND CMAKE_SIZEOF_VOID_P EQUAL 8)
    project(${PROJ} CUDA)
    add_executable(${PROJ} main.cu)
    set_target_properties(${PROJ} PROPERTIES CUDA_ARCHITECTURES native
    CMAKE_CXX_STANDARD 17
    CUDA_STANDARD_REQUIRED ON
    )
endif()