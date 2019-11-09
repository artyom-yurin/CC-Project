#pragma once

#include<string>

class Token {
    public:
        int class_name;
        std::string value;
    public:
        Token();
        Token(int class_name, std::string value);
};
