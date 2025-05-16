function(antlr4_generate_helper
    Antlr4_ProjectTarget
    Antlr4_InputFile
    Antlr4_GeneratorType)
set(options
LISTENER # ARGV3: generate parse tree listener
VISITOR # ARGV4: generate parse tree visitor
)
set(one_value_keywords
PACKAGE # ARGV5: specify a package/namespace for the generated code
LIB # ARGV7: specify a package/namespace for the generated code
)
set(multi_value_keywords
DEPENDS # ARGV6: for Antlr4_ProjectTarget
)
# message("${options}")
# message("${one_value_keywords}")
# message("${multi_value_keywords}")

cmake_parse_arguments(ANTLR4
"${options}"
"${one_value_keywords}"
"${multi_value_keywords}"
${ARGN}
)
antlr4_generate(${Antlr4_ProjectTarget} ${Antlr4_InputFile} ${Antlr4_GeneratorType}
"${ANTLR4_LISTENER}"
"${ANTLR4_VISITOR}"
"${ANTLR4_PACKAGE}"
"${ANTLR4_DEPENDS}"
"${ANTLR4_LIB}"
)
set(ANTLR4_INCLUDE_DIR_${Antlr4_ProjectTarget} ${ANTLR4_INCLUDE_DIR_${Antlr4_ProjectTarget}} PARENT_SCOPE)
set(ANTLR4_SRC_FILES_${Antlr4_ProjectTarget} ${ANTLR4_SRC_FILES_${Antlr4_ProjectTarget}} PARENT_SCOPE)
set(ANTLR4_TOKEN_FILES_${Antlr4_ProjectTarget} ${ANTLR4_TOKEN_FILES_${Antlr4_ProjectTarget}} PARENT_SCOPE)
set(ANTLR4_TOKEN_DIRECTORY_${Antlr4_ProjectTarget} ${ANTLR4_TOKEN_DIRECTORY_${Antlr4_ProjectTarget}} PARENT_SCOPE)
endfunction()
