cmake_path(GET CMAKE_CURRENT_LIST_DIR FILENAME PROJ)

# set(ANTLR4_CPP_GENERATED_SRC_DIR ON)
# set(ANTLR4_GENERATED_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR}/.antlr4)
if(NOT DEFINED MINGW)
    set(CMAKE_CXX_STANDARD 23)
endif()
find_package(antlr4-runtime)
find_package(antlr4-generator)
set(ANTLR4_JAR_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/antlr-${ANTLR_VERSION}-complete.jar)
if(NOT EXISTS ${ANTLR4_JAR_LOCATION})
    file(DOWNLOAD "https://maven.aliyun.com/repository/public/org/antlr/antlr4/${ANTLR_VERSION}/antlr4-${ANTLR_VERSION}-complete.jar" "${ANTLR4_JAR_LOCATION}")
endif()
include(antlr4)
antlr4_generate_helper(${PROJ}_LEXER "${CMAKE_CURRENT_LIST_DIR}/MathLexer.g4" LEXER)

list(APPEND THE_SOURCES ${ANTLR4_SRC_FILES_${PROJ}_LEXER})
antlr4_generate_helper(${PROJ}_PARSER "${CMAKE_CURRENT_LIST_DIR}/MathParser.g4" PARSER
LIB ${ANTLR4_TOKEN_DIRECTORY_${PROJ}_LEXER}
DEPENDS "${ANTLR4_TOKEN_FILES_${PROJ}_LEXER}"
)
list(APPEND THE_SOURCES ${ANTLR4_SRC_FILES_${PROJ}_PARSER})
list(FILTER THE_SOURCES EXCLUDE REGEX "\.(tokens|interp|h)$")
include_directories(${ANTLR4_INCLUDE_DIR} ${ANTLR4_INCLUDE_DIR_${PROJ}_LEXER} ${ANTLR4_INCLUDE_DIR_${PROJ}_PARSER})
add_executable(${PROJ} main.cpp ${THE_SOURCES})
target_link_libraries(${PROJ} antlr4_static)
