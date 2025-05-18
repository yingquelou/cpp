cmake_path(GET CMAKE_CURRENT_LIST_DIR STEM PROJ)
set(Python_USE_STATIC_LIBS TRUE)

# MODULE_NAME 必须是要调试的python扩展名
set(MODULE_NAME utils)

configure_file(debug.conf.h.in debug.conf.h)
find_package(Python REQUIRED COMPONENTS Interpreter Development)
include_directories(${Python_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
add_executable(${PROJ} main.cpp)
target_link_libraries(${PROJ} Python::Python ${MODULE_NAME})