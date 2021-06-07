parser grammar TParser;

options {
	tokenVocab = TLexer;
}

programm : expr_list;

expr_list : (expr? delim)+ ;

expr : if_statement
     | while_statement
     | rvalue
     ;

all_result : ( int_result | float_result );

elsif_statement : ELSIF cond_expr
                  NEWLINE expr_list;

if_statement : IF cond_expr
               NEWLINE expr_list
               NEWLINE (elsif_statement NEWLINE)*
               (ELSE NEWLINE expr_list)? END;

while_statement : WHILE cond_expr NEWLINE expr_list END;

assignment : var=ID op=ASSIGN rvalue;

array_definition : var=ID op=ASSIGN L_SQBRACKET array_definition_elements R_SQBRACKET;

array_definition_elements : ( INT | FLOAT | bool_t) ( COMMA ( INT | FLOAT | bool_t ))* ;

array_selector : ID L_SQBRACKET ( int_result ) R_SQBRACKET;

int_result : int_result op=( MUL | DIV | MOD ) int_result
           | int_result op=( PLUS | MINUS ) int_result
           | L_BRACKET int_result R_BRACKET
           | INT
           ;

float_result : float_result op=( MUL | DIV | MOD ) float_result
             | float_result op=( PLUS | MINUS ) float_result
             | L_BRACKET float_result R_BRACKET
             | FLOAT
             ;

cond_expr : left=comparison op=AND right=cond_expr
          | left=comparison op=OR right=cond_expr
          | L_BRACKET cond_expr R_BRACKET
          | comparison
          ;

comparison : left=comp_var op=( LESS | GREATER | LESS_EQUAL | GREATER_EQUAL ) right=comp_var
           | left=comp_var op=( EQUAL | NOT_EQUAL ) right=comp_var
           ;

comp_var : all_result
         | array_selector
         | ID
         ;

rvalue : ID
       | LITERAL
       | bool_t
       | FLOAT
       | INT
       | array_selector
       | NOT rvalue

       | rvalue ( MUL | DIV | MOD ) rvalue
       | rvalue ( PLUS | MINUS ) rvalue

       | rvalue ( LESS | GREATER | LESS_EQUAL | GREATER_EQUAL ) rvalue

       | rvalue ( EQUAL | NOT_EQUAL ) rvalue

       | rvalue ( OR | AND ) rvalue

       | L_BRACKET rvalue R_BRACKET

       | array_definition
       | assignment
       ;

bool_t : TRUE
     | FALSE
     ;

delim : (SEMICOLON | NEWLINE)+;