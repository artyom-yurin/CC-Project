#include "Lexer.hpp"


Lexer::Lexer(std::string src) {
    this->src = src;
    this->src_iter = this->src.begin();

    createSymbolTable();
}


Token Lexer::next() {
    Token token;

    while (src_iter != src.end()) {
        if (*src_iter == ' ' || *src_iter == '\t') {
            // Skip spaces
        }
        else if (isDigit(*src_iter)) {
            return parseNumber();
        }
        else if (isLetter(*src_iter)) {
            return parseIdentifier();
        }

        src_iter++;
    }
    return token;
}


void Lexer::createSymbolTable() {
    // Feel the table keywords
    std::pair<std::string, ClassName> keywords[] = {
        {"var", ClassName::Var}, {"is", ClassName::Is},
        {"type", ClassName::Type}, {"routine", ClassName::Routine},
        {"end", ClassName::End}, {"record", ClassName::Rec},
        {"array", ClassName::Arr}, {"while", ClassName::While},
        {"loop", ClassName::Loop}, {"for", ClassName::For},
        {"in", ClassName::In}, {"reverse", ClassName::Reverse},
        {"foreach", ClassName::Foreach}, {"from", ClassName::From},
        {"if", ClassName::If}, {"then", ClassName::Then},
        {"else", ClassName::Else}, {"and", ClassName::And},
        {"or", ClassName::Or}, {"xor", ClassName::Xor},
    };

    std::string identifiers[] = {
        "ineger", "real", "boolean",
    };

    for (auto keyword : keywords)
        symbol_table.insert(keyword.first, keyword.second);

    for (std::string identifier : identifiers)
        symbol_table.insert(identifier, ClassName::Ident);

}


bool Lexer::isDigit(char c) {
    return c >= '0' && c <='9';
}


bool Lexer::isLetter(char c) {
    return (c >='a' && c <= 'z') || (c >= 'A' && c <= 'Z');
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
        }
        else if (*src_iter == '.') {
            value += *src_iter;
            src_iter++;

            while (src_iter != src.end() && isDigit(*src_iter)) {
                value += *src_iter;
                src_iter++;
            }
            token.class_name = ClassName::Real;
            token.value = value;
            return token;
        }
        else {
            break;
        }
        src_iter++;
    }
    if (value == "")
        value = "0";

    token.class_name = ClassName::Int;
    token.value = value;
    return token;
}


Token Lexer::parseIdentifier() {
    Token token;
    std::string value = "";

    while(src_iter != src.end() && (isLetter(*src_iter) || isDigit(*src_iter))) {
        value += *src_iter;
        src_iter++;
    }

    ClassName class_name = symbol_table.find(value);

    if (class_name == ClassName::None) {
        class_name = ClassName::Ident;
        symbol_table.insert(value, class_name);
    }

    token.value = value;
    token.class_name = class_name;

    return token;
}


Token Lexer::parseOtherSymbol() {
    Token token = Token(ClassName::None, "");

    if (*src_iter == '+') {
        token.value = "+";
        token.class_name = ClassName::Add;
    }
    else if (*src_iter == '-') {
        token.value = "-";
        token.class_name = ClassName::Sub;
    }
    else if (*src_iter == '*') {
        token.value = "*";
        token.class_name = ClassName::Mul;
    }
    else if (*src_iter == '/') {
        token.value = "/";
        token.class_name = ClassName::Div;
    }
    else if (*src_iter == '%') {
        token.value = "%";
        token.class_name = ClassName::Mod;
    }
    else if (*src_iter == '=') {
        token.value = '=';
        token.class_name = ClassName::Eq;
    }
    else if (*src_iter == '<') {
        if (src_iter + 1 != src.end() && *(src_iter + 1) == '=') {
            src_iter++;
            token.value = "<=";
            token.class_name = ClassName::LessOrEq;
        }
        else {
            token.value = "<";
            token.class_name = ClassName::Less;
        }
    }
    else if (*src_iter == '>') {
        if (src_iter + 1 != src.end() && *(src_iter + 1) == '=') {
            src_iter++;
            token.value = ">=";
            token.class_name = ClassName::GreatOrEq;
        }
        else {
            token.value = ">";
            token.class_name = ClassName::Great;
        }
    }
    else if (*src_iter == '[') {
        token.value = "[";
        token.class_name = ClassName::LefSqBr;
    }
    else if (*src_iter == '(') {
        token.value = "(";
        token.class_name = ClassName::LefBr;
    }
    else if (*src_iter == ']') {
        token.value = "]";
        token.class_name = ClassName::RightSqBr;
    }
    else if (*src_iter == ')') {
        token.value = ")";
        token.class_name = ClassName::RightBr;
    }
    else if (*src_iter == ':') {
        if (src_iter + 1 != src.end() && *(src_iter + 1) == '=') {
            src_iter++;
            token.value = ":=";
            token.class_name = ClassName::Assign;
        }
        else {
            token.value = ":";
            token.class_name = ClassName::Colon;
        }
    }
    else if (*src_iter == ';') {
        token.value = ";";
        token.class_name = ClassName::Semicolon;
    }
    else if (*src_iter == ',') {
        token.value = ",";
        token.class_name = ClassName::Comma;
    }
    else if (*src_iter == '.') {
        token.value = ".";
        token.class_name = ClassName::Dot;
    }
    src_iter++;
    return token;
}
