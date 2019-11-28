cmake_minimum_required(VERSION 3.10)

# set the project name
project(Tutorial)

# add the executable
add_executable(Tutorial
        main.cpp
        )
target_link_libraries(Tutorial
        grammar
        ControlTable
        )

add_library(grammar
        Node.cpp
        )

add_library(ControlTable
        ControlTable.cpp
        )

target_link_libraries(ControlTable
        SymbolTable
        TypeTable
        grammar
        )

add_library(TypeTable
        TypeTable.cpp
        TypeNode.cpp
        )

target_link_libraries(TypeTable
        SymbolTable
        grammar
        )

add_library(SymbolTable
        SymbolTable.cpp
        SymbolNode.cpp
        )

target_link_libraries(SymbolTable
        TypeTable
        grammar
        )
