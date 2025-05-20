# 当前目录下,除py目标外,其余目标都是python扩展
# py目标可通过gdb从c/c++代码层面调试python扩展
# 通过设置MODULE_NAME,指定需要调试python扩展
# 被调试python扩展将以静态库的方式构建
# 当前目录下的目标都应遵循上述约定
# set(MODULE_NAME spam)

set(Python_USE_STATIC_LIBS TRUE)
if(WIN32)
    set(Python_EXECUTABLE_DEBUG ON)
endif()

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