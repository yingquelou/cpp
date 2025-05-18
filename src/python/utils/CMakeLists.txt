cmake_policy(PUSH)
cmake_policy(SET CMP0078 NEW)
cmake_policy(SET CMP0086 NEW)
cmake_path(GET CMAKE_CURRENT_LIST_DIR STEM PROJ)
# file(GLOB PROJ_SOURCES RELATIVE ${CMAKE_CURRENT_LIST_DIR} *.c;*.C;*.c++;*.cc;*.cpp;*.cxx;*.cu;*.mpp;*.m;*.M;*.mm;*.ixx;*.cppm;*.ccm;*.cxxm;*.c++m;*.h;*.hh;*.h++;*.hm;*.hpp;*.hxx;*.in;*.txx;*.f;*.F;*.for;*.f77;*.f90;*.f95;*.f03;*.hip;*.ispc)
find_package(Python REQUIRED COMPONENTS Development Interpreter)
include_directories(${Python_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
find_package(SWIG REQUIRED)
include(UseSWIG)
set_property(SOURCE utils.i PROPERTY
CPLUSPLUS ON)
set_property(SOURCE utils.i 
PROPERTY COMPILE_OPTIONS 
"-module" "${PROJ}"
"-directors"
# "-oh" "ok.h"
)
if(MODULE_NAME)
    set(DEBUG_MODULE TYPE STATIC)
endif()

swig_add_library(${PROJ} 
${DEBUG_MODULE}
# NO_PROXY
LANGUAGE python SOURCES utils.i)
target_link_libraries(${PROJ} Python::Python)
install(TARGETS ${PROJ} DESTINATION ${CMAKE_SOURCE_DIR}/.venv/lib/python3.12/site-packages)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/utils.py DESTINATION ${CMAKE_SOURCE_DIR}/.venv/lib/python3.12/site-packages)
cmake_policy(POP)