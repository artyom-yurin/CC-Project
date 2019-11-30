set (bison_cmd "bison")
execute_process(COMMAND ${bison_cmd} -d --output Parser.cpp ${CMAKE_CURRENT_SOURCE_DIR}/grammar.ypp
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_library(Parser
        Parser.cpp)

target_link_libraries(Parser
        Lexer
        common
        )
