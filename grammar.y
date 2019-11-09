%{
#include <cstdio>
#include <stdio.h>
#include <iostream>
#include <string>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"
int yylex(void);
extern "C" FILE *yyin;

#define YYSTYPE Token*
Lexer* lexer;
void yyerror(char const *s);
%}

%token VAR IS
%token ROUTINE END
%token IDENTIFIER TYPE
%token INTEGER REAL BOOLEAN
%token RECORD ARRAY
%token ASSIGNMENT_SIGN RANGE_SIGN
%token WHILE LOOP FOR
%token REVERSE IN
%token IF THEN ELSE
%token AND OR XOR
%token LT_SIGN LET_SIGN GT_SIGN GET_SIGN EQ_SIGN NEQ_SIGN
%token MULT_SIGN DIV_SIGN MOD_SIGN
%token PLUS_SIGN MINUS_SIGN
%token TRUE FALSE REAL_LITERAL INTEGER_LITERAL
%token RETURN
%token L_SQ_BR R_SQ_BR L_BR R_BR
%token COLON DOT COMMA

%start program
%%
program
    :
    | program simple_declaration
    | program routine_declaration
    ;

simple_declaration
    : variable_declarartion
    | type_declaration
    ;

variable_declarartion
    : VAR IDENTIFIER COLON type variable_expression
    | VAR IDENTIFIER IS expression
    ;

variable_expression
    :
    | IS expression
    ;

type_declaration
    : TYPE IDENTIFIER IS type
    ;

routine_declaration
    : ROUTINE IDENTIFIER L_BR routine_parameters R_BR routine_return_type IS body END
    ;

routine_return_type
    :
    | COLON type
    ;

routine_parameters
    :
    | parameters
    ;

parameters
    : parameters COMMA parameter_declaration
    | parameter_declaration
    ;

parameter_declaration
    : IDENTIFIER COLON IDENTIFIER
    ;

type
    : primitive_type
    | array_type
    | record_type
    | IDENTIFIER
    ;

primitive_type
    : INTEGER
    | REAL
    | BOOLEAN
    ;

record_type
    : RECORD variables_declarartion END
    ;

variables_declarartion
    : variable_declarartion variables_declarartion
    |
    ;

array_type
    : ARRAY L_SQ_BR expression R_SQ_BR type
    ;

body
    :
    | body simple_declaration
    | body statement
    ;

statement
    : assignment
    | routine_call
    | while_loop
    | for_loop
    | if_statement
    | return
    ;

return
    : RETURN return_value
    ;

return_value
    :
    | expression
    ;

assignment
    : modifiable_primary ASSIGNMENT_SIGN expression
    ;

routine_call
    : IDENTIFIER L_BR arguments R_BR
    ;

arguments
    :
    | expressions
    ;

expressions
    : expressions COMMA expression
    | expression
    ;

while_loop
    : WHILE expression LOOP body END
    ;

for_loop
    : FOR IDENTIFIER range LOOP body END
    ;

range
    : IN reverse expression RANGE_SIGN expression
    ;

reverse
    :
    | REVERSE
    ;

if_statement
    : IF expression THEN body else_body END
    ;

else_body
    :
    | ELSE body
    ;

expression
    : expression logic_operation relation
    | relation
    ;

logic_operation
    : AND
    | OR
    | XOR
    ;

relation
    : simple
    | simple compare_sign simple
    ;

compare_sign
    : LT_SIGN
    | LET_SIGN
    | GT_SIGN
    | GET_SIGN
    | EQ_SIGN
    | NEQ_SIGN
    ;

simple
    : simple mult_sign_f factor
    | factor
    ;
// f mean first priority
mult_sign_f
    : MULT_SIGN
    | DIV_SIGN
    | MOD_SIGN
    ;

factor
    : factor mult_sign_s summand
    | summand
    ;
// s mean second priority
mult_sign_s
    : PLUS_SIGN
    | MINUS_SIGN
    ;

summand
    : primary
    | L_BR expression R_BR
    ;

primary
    : TRUE
    | FALSE
    | REAL_LITERAL
    | INTEGER_LITERAL
    | modifiable_primary
    ;

modifiable_primary
    : IDENTIFIER
    | modifiable_primary L_SQ_BR expression R_SQ_BR
    | modifiable_primary DOT modifiable_primary
    ;
%%

int yylex()
{
  if (lexer == nullptr) return 0;
  Token currentToken = lexer->next();
  int tokenType = currentToken.class_name;
  if (tokenType == 0) {return 0;}
  yylval = new Token(tokenType, currentToken.value);
  return tokenType;
}

void yyerror(char const *s)
{
	fflush(stdout);
	printf("\n%s\n", "PIZDEC");
}

int main(int argc, char** argv){
    if (argc != 2) {
        std::cerr << "Invalid number of args" << std::endl;
        std::cerr << "Usage: " << argv[0] << " <path_to_source>" << std::endl;
        return 1;
    }

    std::ifstream src_file(argv[1]);
    std::stringstream buffer;

    buffer << src_file.rdbuf();

    lexer = new Lexer(buffer.str());

    yyparse();

    return 0;
}
