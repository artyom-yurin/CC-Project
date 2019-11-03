#pragma once

#include<string>

enum ClassName {
    None,   // Special class meaning that token is empty
    Ident,  // Identificator
    True, False,    // Special values
    Routine, End,   // Procedure call
    If, Then, Else, // Conditional keywords
    Not, And, Or, Xor,  // Logical operators
    Real, Int, Boolean, // Literals
    Type, Var, Arr, Rec,  // Type keywords
    Add, Sub, Mul, Mod, Div, // Math operations
    BoolType, IntType, RealType,    // Basic Types
    Colon, Semicolon, Assign, Comma, Dot,   // Special symbols
    While, For, Loop, Reverse, Foreach, From,  // Loop keywords
    Eq, NoEq, Less, LessOrEq, Great, GreatOrEq, Is, In,  // Comparison operators
    LefBr, RightBr, LefSqBr, RightSqBr, LefFigBr, RightFigBr,  // Brackets
};


class Token {
    public:
        ClassName class_name;
        std::string value;
    public:
        Token();
        Token(ClassName class_name, std::string value);
};
