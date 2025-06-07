cmake_path(GET CMAKE_CURRENT_LIST_DIR FILENAME PROJ)
# set(CUDA_ARCHITECTURES)
find_package(CUDA)
if(CUDA_FOUND)
    add_executable(${PROJ} main.cu)
    set_target_properties(${PROJ} PROPERTIES CUDA_ARCHITECTURES all-major
    CMAKE_CXX_STANDARD 17
    )
endif()

