#include "Token.hpp"

Token::Token() {
    this->class_name = -1;
}

Token::Token(int class_name, std::string value) {
    this->class_name = class_name;
    this->value = value;
}
