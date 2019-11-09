#!/usr/bin/env bash
bison -d --output yacc.cpp grammar.y
g++ yacc.cpp ./lexer/Lexer.cpp ./lexer/Token.cpp ./lexer/SymbolTable.cpp -o parser
rm yacc.cpp yacc.hpp
./parser test.txt
