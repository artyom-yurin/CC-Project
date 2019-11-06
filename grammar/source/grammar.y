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
%token LT_SIGN, LET_SIGN, GT_SIGN, GET_SIGN, EQ_SIGN, NEQ_SIGN
%token MULT_SIGN DIV_SIGN MOD_SIGN
%token PLUS_SIGN MINUS_SIGN
%token TRUE FALSE REAL_LITERAL INTEGER_LITERAL
%token RETURN

%start program
// {} group elements that can be repeated 0 or more times
// [] group optional elements (repeated 0 or 1 time)
// | denotes alternatives
// () simply group alternatives 2•Only three relational operators
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
    : VAR IDENTIFIER ':' type variable_expression
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
    : ROUTINE IDENTIFIER '(' routine_parameters ')' routine_return_type IS body END
    ;

routine_return_type
    :
    | ':' type
    ;

routine_parameters
    :
    | parameters
    ;

parameters
    : parameters ',' parameter_declaration
    | parameter_declaration
    ;

parameter_declaration
    : IDENTIFIER ':' IDENTIFIER
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
    : ARRAY '[' expression ']' type
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
    : IDENTIFIER '(' arguments ')'
    ;

arguments
    :
    | expressions
    ;

expressions
    : expressions ',' expression
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
    | '(' expression ')'
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
    | modifiable_primary '[' expression ']'
    | modifiable_primary '.' modifiable_primary
    ;
%%