# cmake_policy(PUSH)
# cmake_policy(SET CMP0084 NEW)
cmake_path(GET CMAKE_CURRENT_LIST_DIR FILENAME PROJ)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS AxContainer Core Gui Widgets)
if(Qt6_FOUND)
    qt_standard_project_setup()

    qt_add_executable(${PROJ}
        main.cpp
    )
    target_link_libraries(${PROJ} PRIVATE Qt6::AxContainer Qt6::Widgets Qt6::Core)
endif()

# cmake_policy(POP)