%{
#include <cstdio>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <stdarg.h>
#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"
#include "Node.hpp"
int yylex(void);
extern "C" FILE *yyin;

#define YYSTYPE CNode*

Lexer* lexer;
void yyerror(char const *s);
CNode *root;
CNode* add_node(const std::string& name, int argc, ...);
void print_tree(CNode* root);
void print_node(CNode* node, int margin);
%}

// %union {
//     int number;
//     char symbol;
// 	char string[30];
// 	struct nodeType *node;
// };

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

// %type <node> program simple_declaration variable_declarartion variable_expression type_declaration routine_declaration routine_return_type
// %type <node> routine_parameters parameters parameter_declaration type primitive_type
// %type <node> record_type variables_declarartion array_type body statement return
// %type <node> return_value assignment routine_call arguments expressions while_loop
// %type <node> for_loop range reverse if_statement else_body expression logic_operation relation
// %type <node> compare_sign simple mult_sign_f factor mult_sign_s summand primary modifiable_primary



%start program
%%
program
    : {$$ = nullptr;}
    | program simple_declaration { $$ = add_node("program", 1, $2); root = $$;}
    | program routine_declaration { $$ = add_node("program", 1, $2); root = $$;}
    ;

simple_declaration
    : variable_declarartion { $$ = add_node("simple_declarartion", 1, $1); }
    | type_declaration { $$ = add_node("simple_declaration", 1, $1); }
    ;

variable_declarartion
    : VAR IDENTIFIER COLON type variable_expression { $$ = add_node("variable_declarartion", 3, yylval -> name, $4, $5);}
    | VAR IDENTIFIER IS expression { $$ = add_node("variable_declarartion", 2, yylval -> name, $4);}
    ;

variable_expression
    : {$$ = nullptr;}
    | IS expression { $$ = add_node("variable_expression", 1, $2);}
    ;

type_declaration
    : TYPE IDENTIFIER IS type { $$ = add_node("type_declaration", 2, yylval -> name, $4);}
    ;

routine_declaration
    : ROUTINE IDENTIFIER L_BR routine_parameters R_BR routine_return_type IS body END { $$ = add_node("routine_declaration", 4, yylval -> name, $4, $6, $8);}
    ;

routine_return_type
    : {$$ = nullptr;}
    | COLON type { $$ = add_node("routine_return_type", 1, $2);}
    ;

routine_parameters
    : {$$ = nullptr;}
    | parameters { $$ = add_node("routine_parameters", 1, $1);}
    ;

parameters
    : parameters COMMA parameter_declaration { $$ = add_node("parameters", 2, $1, $3);}
    | parameter_declaration { $$ = add_node("parameters", 1, $1);}
    ;

parameter_declaration
    : IDENTIFIER COLON IDENTIFIER { $$ = add_node("parameter_declaration", 2, yylval -> name, yylval -> name);}
    ;

type
    : primitive_type { $$ = add_node("type", 1, $1);}
    | array_type { $$ = add_node("type", 1, $1);}
    | record_type { $$ = add_node("type", 1, $1);}
    | IDENTIFIER { $$ = add_node("type", 1, yylval -> name);}
    ;

primitive_type
    : INTEGER { $$ = add_node("primitive_type", 1, yylval -> name);}
    | REAL { $$ = add_node("primitive_type", 1, yylval -> name);}
    | BOOLEAN { $$ = add_node("primitive_type", 1, yylval -> name);}
    ;

record_type
    : RECORD variables_declarartion END { $$ = add_node("record_type", 1, $2);}
    ;

variables_declarartion
    : variable_declarartion variables_declarartion { $$ = add_node("variables_declarartion", 2, $1, $2);}
    | {$$ = nullptr;}
    ;

array_type
    : ARRAY L_SQ_BR expression R_SQ_BR type { $$ = add_node("array_type", 2, $3, $5);}
    ;

body
    : {$$ = nullptr;}
    | body simple_declaration { $$ = add_node("body", 2, $1, $2);}
    | body statement { $$ = add_node("body", 2, $1, $2);}
    ;

statement
    : assignment  { $$ = add_node("statement", 1, $1);}
    | routine_call { $$ = add_node("statement", 1, $1);}
    | while_loop { $$ = add_node("statement", 1, $1);}
    | for_loop { $$ = add_node("statement", 1, $1);}
    | if_statement { $$ = add_node("statement", 1, $1);}
    | return { $$ = add_node("statement", 1, $1);}
    ;

return
    : RETURN return_value { $$ = add_node("return", 1, $2); }
    ;

return_value
    : {$$ = nullptr;}
    | expression { $$ = add_node("return_value", 1, $1);}
    ;

assignment
    : modifiable_primary ASSIGNMENT_SIGN expression { $$ = add_node("assignment", 2, $1, $3);}
    ;

routine_call
    : IDENTIFIER L_BR arguments R_BR { $$ = add_node("routine_call", 2, yylval -> name, $3);}
    ;

arguments
    : {$$ = nullptr;}
    | expressions { $$ = add_node("arguments", 1, $1);}
    ;

expressions
    : expressions COMMA expression { $$ = add_node("expressions", 2, $1, $3);}
    | expression { $$ = add_node("expressions", 1, $1);}
    ;

while_loop
    : WHILE expression LOOP body END { $$ = add_node("while_loop", 2, $2, $4);}
    ;

for_loop
    : FOR IDENTIFIER range LOOP body END { $$ = add_node("for_loop", 3, yylval -> name, $3, $5);}
    ;

range
    : IN reverse expression RANGE_SIGN expression { $$ = add_node("range", 3, $2, $3, $5);}
    ;

reverse
    : {$$ = nullptr;}
    | REVERSE { $$ = add_node("reverse", 1, yylval -> name);}
    ;

if_statement
    : IF expression THEN body else_body END { $$ = add_node("if_statement", 3, $2, $4, $5);}
    ;

else_body
    : {$$ = nullptr;}
    | ELSE body { $$ = add_node("else_body", 1, $2);}
    ;

expression
    : expression logic_operation relation { $$ = add_node("expression", 3, $1, $2, $3);}
    | relation { $$ = add_node("expression", 1, $1);}
    ;

logic_operation
    : AND { $$ = add_node("logic_operation", 1, yylval -> name);}
    | OR { $$ = add_node("logic_operation", 1, yylval -> name);}
    | XOR { $$ = add_node("logic_operation", 1, yylval -> name);}
    ;

relation
    : simple { $$ = add_node("relation", 1, $1);}
    | simple compare_sign simple { $$ = add_node("relation", 3, $1, $2, $3);}
    ;

compare_sign
    : LT_SIGN { $$ = add_node("compare_sign", 1, yylval -> name);}
    | LET_SIGN { $$ = add_node("compare_sign", 1, yylval -> name);}
    | GT_SIGN { $$ = add_node("compare_sign", 1, yylval -> name);}
    | GET_SIGN { $$ = add_node("compare_sign", 1, yylval -> name);}
    | EQ_SIGN { $$ = add_node("compare_sign", 1, yylval -> name);}
    | NEQ_SIGN { $$ = add_node("compare_sign", 1, yylval -> name);}
    ;

simple
    : simple mult_sign_f factor { $$ = add_node("simple", 3, $1, $2, $3);}
    | factor { $$ = add_node("simple", 1, $1);}
    ;
// f mean first priority
mult_sign_f
    : MULT_SIGN { $$ = add_node("mult_sign_f", 1, yylval -> name);}
    | DIV_SIGN { $$ = add_node("mult_sign_f", 1, yylval -> name);}
    | MOD_SIGN { $$ = add_node("mult_sign_f", 1, yylval -> name);}
    ;

factor
    : factor mult_sign_s summand { $$ = add_node("factor", 3, $1, $2, $3);}
    | summand { $$ = add_node("factor", 1, $1);}
    ;
// s mean second priority
mult_sign_s
    : PLUS_SIGN { $$ = add_node("mult_sign_s", 1, yylval -> name);}
    | MINUS_SIGN { $$ = add_node("mult_sign_s", 1, yylval -> name);}
    ;

summand
    : primary { $$ = add_node("summand", 1, $1);}
    | L_BR expression R_BR { $$ = add_node("summand", 1, $2);}
    ;

primary
    : TRUE { $$ = add_node("primary", 1, yylval -> name);}
    | FALSE { $$ = add_node("primary", 1, yylval -> name);}
    | REAL_LITERAL { $$ = add_node("primary", 1, yylval -> name);}
    | INTEGER_LITERAL { $$ = add_node("primary", 1, yylval -> name);}
    | modifiable_primary { $$ = add_node("primary", 1, $1);}
    ;

modifiable_primary
    : IDENTIFIER { $$ = add_node("modifiable_primary", 1, yylval -> name);}
    | modifiable_primary L_SQ_BR expression R_SQ_BR { $$ = add_node("modifiable_primary", 2, $1, $3);}
    | modifiable_primary DOT modifiable_primary { $$ = add_node("modifiable_primary", 2, $1, $3);}
    ;
%%

int yylex()
{
  if (lexer == nullptr) return 0;
  Token currentToken = lexer->next();
  int tokenType = currentToken.class_name;
  if (tokenType == 0) {return 0;}
  yylval = new CNode(currentToken.value);
  return tokenType;
}

void yyerror(char const *s)
{
	fflush(stdout);
	printf("\n%s\n", "PIZDEC");
}

CNode* add_node(const std::string& name, int argc, ...) {
	va_list argp;
  CNode* newNode = new CNode(name);

  va_start(argp, argc);
  for (int i = 0; i < argc; i++)
        newNode->children.push_back(va_arg(argp, CNode*));
  va_end(argp);

  return newNode;
}

void print_node(CNode* node, int margin) {
  if (node == nullptr) return;
  for (int i = 0; i < margin; i++)
			std::cout << "   ";
  std::cout << "<" << node->name << ">\n";
  for (int j = 0; j < node->children.size(); j++)
	  print_node(node->children[j], margin + 1);
}

void print_tree(CNode* root) {
  	print_node(root, 0);
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

    print_tree(root);

    return 0;
}
