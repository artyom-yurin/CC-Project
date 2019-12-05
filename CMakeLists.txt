cmake_minimum_required(VERSION 3.10)

# set the project name
project(ICompiler)

SET(BASEPATH "${CMAKE_SOURCE_DIR}")
INCLUDE_DIRECTORIES("${BASEPATH}")

# add the executable
add_subdirectory(lexer)
add_subdirectory(common)
add_subdirectory(grammar)
add_subdirectory(semantic_analyzer)

configure_file(test.txt ${CMAKE_BINARY_DIR} COPYONLY)

add_executable(ICompiler
        main.cpp
        )
target_link_libraries(ICompiler
        common
        Parser
        Lexer
        ControlTable
        )
