#pragma once

#include "LexerSymbolTable.hpp"
#include "Token.hpp"
#include <iostream>
#include <string>

class Lexer {
private:
  std::string src;
  std::string::iterator src_iter;

public:
  LexerSymbolTable symbol_table;

public:
  Lexer(std::string src);
  Token next();
  int column;
private:
  void createSymbolTable();

  bool isDigit(char c);
  bool isHexDigit(char c);
  bool isLetter(char c);
  bool isOtherSymbol(char c);

  void parseComment();
  Token parseString();
  Token parseOtherSymbol();
  Token parseNumber();
  Token parseIdentifier();
  void count(std::string str);
};
