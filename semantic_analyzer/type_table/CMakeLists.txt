add_library(TypeTable
        TypeTable.cpp
        TypeNode.cpp
        )

target_link_libraries(TypeTable
        SymbolTable
        common
        )
