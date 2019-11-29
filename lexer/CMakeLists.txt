add_library(Lexer
        Lexer.cpp
        LexerSymbolTable.cpp
        Token.cpp
        )
target_link_libraries(Lexer
        Parser
        )
