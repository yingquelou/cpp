cmake_policy(PUSH)
cmake_policy(SET CMP0078 NEW)
cmake_policy(SET CMP0086 NEW)
cmake_path(GET CMAKE_CURRENT_LIST_DIR STEM PROJ)
# file(GLOB PROJ_SOURCES RELATIVE ${CMAKE_CURRENT_LIST_DIR} *.c;*.C;*.c++;*.cc;*.cpp;*.cxx;*.cu;*.mpp;*.m;*.M;*.mm;*.ixx;*.cppm;*.ccm;*.cxxm;*.c++m;*.h;*.hh;*.h++;*.hm;*.hpp;*.hxx;*.in;*.txx;*.f;*.F;*.for;*.f77;*.f90;*.f95;*.f03;*.hip;*.ispc)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
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
if(MODULE_NAME STREQUAL PROJ)
    set(DEBUG_MODULE TYPE STATIC NO_PROXY)
endif()

swig_add_library(${PROJ} 
${DEBUG_MODULE}
LANGUAGE python SOURCES utils.i)
target_link_libraries(${PROJ} Python::Python)
# if(NOT MODULE_NAME STREQUAL PROJ)
#     install(TARGETS ${PROJ} DESTINATION ${Python_SITELIB})
#     install(FILES ${CMAKE_CURRENT_BINARY_DIR}/utils.py DESTINATION ${Python_SITELIB})
# endif()
cmake_policy(POP)