add_subdirectory("symbol_table")
add_subdirectory("type_table")

add_library(ControlTable
        ControlTable.cpp
        )

target_link_libraries(ControlTable
        SymbolTable
        TypeTable
        common
        )

add_library(Analyzer
        CAnalyzer.cpp
        )

target_link_libraries(Analyzer
        ControlTable
        common
        )
