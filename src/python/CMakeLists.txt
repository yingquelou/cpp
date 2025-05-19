# 当前目录下,除py子目录外,其余目录都是python扩展
# py子目录可通过gdb从c/c++代码层面调试自定义的python扩展
# MODULE_NAME 必须是要调试的python扩展名,一般是当前目录相应的目录名
# 当前cmake配置中,目录名与python扩展名对应
set(MODULE_NAME spam)

set(Python_USE_STATIC_LIBS TRUE)
if(WIN32)
    set(Python_EXECUTABLE_DEBUG ON)
endif()
# Python_EXECUTABLE可以指向虚拟环境的python
set(Python_EXECUTABLE ${CMAKE_SOURCE_DIR}/.venv/bin/python)

find_package(Python REQUIRED COMPONENTS Interpreter Development)
include_directories(${Python_INCLUDE_DIRS})

show_variable(Python_LINK_OPTIONS)
show_variable(Python_STDLIB)
show_variable(Python_STDARCH)
show_variable(Python_SITELIB)
show_variable(Python_SITEARCH)
show_variable(Python_RUNTIME_LIBRARY_DIRS)
show_variable(Python_EXECUTABLE)

router_directories()