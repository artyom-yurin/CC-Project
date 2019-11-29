#include "Lexer.hpp"
#include "grammar/Parser.hpp"

using namespace yy;

Lexer::Lexer(std::string src) {
  this->src = src;
  this->src_iter = this->src.begin();
  this->column = 0;
  createSymbolTable();
}

Token Lexer::next() {
  Token token(0, "");
  while (src_iter != src.end()) {
    if (*src_iter == ' ' || *src_iter == '\t' || *src_iter == '\n') {
      count(std::string(1, *src_iter));
      // Skip spaces
    } else if (isDigit(*src_iter)) {
      token = parseNumber();
      break;
    } else if (isLetter(*src_iter)) {
      token = parseIdentifier();
      break;
    } else if (isOtherSymbol(*src_iter)) {
      token = parseOtherSymbol();
      break;
    }

    src_iter++;
  }
  count(token.value);
  return token;
}

void Lexer::count(std::string str) {
  int i;

  for (i = 0; i < str.size(); i++)
    if (str[i] == '\n')
      column = 0;
    else if (str[i] == '\t')
      column += 8 - (column % 8);
    else
      column++;

  std::cout << str;
}

void Lexer::createSymbolTable() {
  // Feel the table keywords
  std::pair<std::string, int> keywords[] = {
      {"var", parser::token::yytokentype::VAR},
      {"is", parser::token::yytokentype::IS},
      {"type", parser::token::yytokentype::TYPE},
      {"routine", parser::token::yytokentype::ROUTINE},
      {"end", parser::token::yytokentype::END},
      {"record", parser::token::yytokentype::RECORD},
      {"array", parser::token::yytokentype::ARRAY},
      {"while", parser::token::yytokentype::WHILE},
      {"loop", parser::token::yytokentype::LOOP},
      {"for", parser::token::yytokentype::FOR},
      {"in", parser::token::yytokentype::IN},
      {"reverse", parser::token::yytokentype::REVERSE},
      {"return", parser::token::yytokentype::RETURN},
      {"if", parser::token::yytokentype::IF},
      {"then", parser::token::yytokentype::THEN},
      {"else", parser::token::yytokentype::ELSE},
      {"and", parser::token::yytokentype::AND},
      {"or", parser::token::yytokentype::OR},
      {"xor", parser::token::yytokentype::XOR},
      {"integer", parser::token::yytokentype::INTEGER},
      {"real", parser::token::yytokentype::REAL},
      {"boolean", parser::token::yytokentype::BOOLEAN},
      {"true", parser::token::yytokentype::TRUE},
      {"false", parser::token::yytokentype::FALSE},
      {"not", parser::token::yytokentype::NOT},
  };

  for (auto keyword : keywords)
    symbol_table.insert(keyword.first, keyword.second);
}

bool Lexer::isDigit(char c) { return c >= '0' && c <= '9'; }

bool Lexer::isLetter(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool Lexer::isOtherSymbol(char c) {
  return (c >= '(' && c <= '/') || (c >= ':' && c <= '>') || (c == '[') ||
         (c = ']') || (c == '{') || (c == '}') || (c == '%');
}

Token Lexer::parseNumber() {
  Token token;
  std::string value = "";

  // Skip zeros
  while (src_iter != src.end() && *src_iter == '0') {
    src_iter++;
  }

  while (true) {
    if (isDigit(*src_iter)) {
      value += *src_iter;
    } else if (*src_iter == '.') {
      value += *src_iter;
      src_iter++;

      while (src_iter != src.end() && isDigit(*src_iter)) {
        value += *src_iter;
        src_iter++;
      }
      token.class_name = parser::token::yytokentype::REAL_LITERAL;
      token.value = value;
      return token;
    } else {
      break;
    }
    src_iter++;
  }
  if (value == "")
    value = "0";

  token.class_name = parser::token::yytokentype::INTEGER_LITERAL;
  token.value = value;
  return token;
}

Token Lexer::parseIdentifier() {
  Token token;
  std::string value = "";

  while (src_iter != src.end() && (isLetter(*src_iter) || isDigit(*src_iter))) {
    value += *src_iter;
    src_iter++;
  }

  int class_name = symbol_table.find(value);

  if (class_name == -1) {
    class_name = parser::token::yytokentype::IDENTIFIER;
    symbol_table.insert(value, parser::token::yytokentype::IDENTIFIER);
  }

  token.value = value;
  token.class_name = class_name;

  return token;
}

Token Lexer::parseOtherSymbol() {
  Token token = Token(-1, "");

  if (*src_iter == '+') {
    token.value = "+";
    token.class_name = parser::token::yytokentype::PLUS_SIGN;
  } else if (*src_iter == '-') {
    token.value = "-";
    token.class_name = parser::token::yytokentype::MINUS_SIGN;
  } else if (*src_iter == '*') {
    token.value = "*";
    token.class_name = parser::token::yytokentype::MULT_SIGN;
  } else if (*src_iter == '/') {
    if (*(src_iter + 1) == '=' && (src_iter + 1 != src.end())) {
      token.value = "/=";
      token.class_name = parser::token::yytokentype::NEQ_SIGN;
    } else {
      token.value = "/";
      token.class_name = parser::token::yytokentype::DIV_SIGN;
    }
  } else if (*src_iter == '%') {
    token.value = "%";
    token.class_name = parser::token::yytokentype::MOD_SIGN;
  } else if (*src_iter == '=') {
    token.value = '=';
    token.class_name = parser::token::yytokentype::EQ_SIGN;
  } else if (*src_iter == '<') {
    if (src_iter + 1 != src.end() && *(src_iter + 1) == '=') {
      src_iter++;
      token.value = "<=";
      token.class_name = parser::token::yytokentype::LET_SIGN;
    } else {
      token.value = "<";
      token.class_name = parser::token::yytokentype::LT_SIGN;
    }
  } else if (*src_iter == '>') {
    if (src_iter + 1 != src.end() && *(src_iter + 1) == '=') {
      src_iter++;
      token.value = ">=";
      token.class_name = parser::token::yytokentype::GET_SIGN;
    } else {
      token.value = ">";
      token.class_name = parser::token::yytokentype::GT_SIGN;
    }
  } else if (*src_iter == '[') {
    token.value = "[";
    token.class_name = parser::token::yytokentype::L_SQ_BR;
  } else if (*src_iter == '(') {
    token.value = "(";
    token.class_name = parser::token::yytokentype::L_BR;
  } else if (*src_iter == ']') {
    token.value = "]";
    token.class_name = parser::token::yytokentype::R_SQ_BR;
  } else if (*src_iter == ')') {
    token.value = ")";
    token.class_name = parser::token::yytokentype::R_BR;
  } else if (*src_iter == ':') {
    if (src_iter + 1 != src.end() && *(src_iter + 1) == '=') {
      src_iter++;
      token.value = ":=";
      token.class_name = parser::token::yytokentype::ASSIGNMENT_SIGN;
    } else {
      token.value = ":";
      token.class_name = parser::token::yytokentype::COLON;
    }
  } else if (*src_iter == ',') {
    token.value = ",";
    token.class_name = parser::token::yytokentype::COMMA;
  } else if (*src_iter == '.') {
    token.value = ".";
    token.class_name = parser::token::yytokentype::DOT;
  }
  src_iter++;
  return token;
}
