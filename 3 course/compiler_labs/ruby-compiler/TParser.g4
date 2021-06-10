parser grammar TParser;

options {
	tokenVocab = TLexer;
}

programm : statement_list EOF;

statement_list : ( statement delim* )*;

statement : if_statement
          | while_statement
          | puts
          | expr
          ;

elsif_statement : ELSIF expr
                  NEWLINE statement_list;

if_statement : IF expr
               NEWLINE statement_list
               NEWLINE (elsif_statement NEWLINE)*
               (ELSE NEWLINE statement_list)? END;

while_statement : WHILE expr NEWLINE statement_list END;

assignment : (ID | array_selector) op=ASSIGN expr;

array_definition : L_SQBRACKET array_definition_elements R_SQBRACKET;

array_definition_elements : ( expr ) ( COMMA ( expr ))* ;

array_selector : ID L_SQBRACKET expr R_SQBRACKET;

expr : ID
       | LITERAL
       | bool_t
       | FLOAT
       | INT
       | GETS
       // круглые скобки
       | L_BRACKET expr R_BRACKET
       // взятие индекса
       | array_selector
       // логическое НЕ
       | NOT expr
       // умножение
       | expr ( MUL | DIV | MOD ) expr
       // сложение
       | expr ( PLUS | MINUS ) expr
       // сравнение
       | expr ( LESS | GREATER | LESS_EQUAL | GREATER_EQUAL ) expr
       // равенство / неравенство
       | expr ( EQUAL | NOT_EQUAL ) expr
       // логическое И
       | expr AND expr
       // логическое ИЛИ
       | expr OR expr
       // объявление массива
       | array_definition
       // присваивание
       | assignment
       // ввод с клавиатуры
       ;

bool_t : TRUE
       | FALSE
       ;

puts : PUTS expr;

delim : (SEMICOLON | NEWLINE)+;