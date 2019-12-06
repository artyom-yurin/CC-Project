add_library(Generator
        CGenerator.cpp)

target_link_libraries(Generator
        IndexTable
        common
        )

add_library(IndexTable
        IndexTable.cpp)
