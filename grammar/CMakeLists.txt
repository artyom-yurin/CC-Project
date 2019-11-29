add_library(Parser
        Parser.cpp)

target_link_libraries(Parser
        Lexer
        common
        )
