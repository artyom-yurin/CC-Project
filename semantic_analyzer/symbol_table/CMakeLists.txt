add_library(SymbolTable
        SymbolTable.cpp
        SymbolNode.cpp
        )

target_link_libraries(SymbolTable
        TypeTable
        common
        )
