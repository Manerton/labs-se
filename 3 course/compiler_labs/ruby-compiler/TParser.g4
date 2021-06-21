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

if_statement : IF expr
               NEWLINE statement_list
               (ELSE NEWLINE statement_list)? END;

while_statement : WHILE expr NEWLINE statement_list END;

assignment : (ID | array_selector) ASSIGN expr;

array_declaration : ARRAY_DECLARATION L_BRACKET expr COMMA expr R_BRACKET;

array_definition : L_SQBRACKET array_definition_elements R_SQBRACKET;

array_definition_elements : ( expr ) ( COMMA ( expr ))* ;

array_selector : ID L_SQBRACKET expr R_SQBRACKET;

expr : ID                                                               #idExpr
       | (TRUE | FALSE)                                                 #boolExpr
       | FLOAT                                                          #floatExpr
       | INT                                                            #intExpr
       | UGETS                                                          #ugetsExpr
       | IGETS                                                          #igetsExpr
       | FGETS                                                          #fgetsExpr
       | MINUS expr                                                     #unaryMinusExpr
       // круглые скобки
       | L_BRACKET expr R_BRACKET                                       #bracketsExpr
       // взятие индекса
       | array_selector                                                 #arrSelExpr
       // логическое НЕ
       | NOT expr                                                       #logicExpr
       // умножение
       | expr op=( MUL | DIV ) expr                                     #arifExpr
       // сложение
       | expr op=( PLUS | MINUS ) expr                                  #arifExpr
       // сравнение
       | expr op=( LESS | GREATER | LESS_EQUAL | GREATER_EQUAL ) expr   #compExpr
       // равенство / неравенство
       | expr op=( EQUAL | NOT_EQUAL ) expr                             #compExpr
       // логическое И
       | expr AND expr                                                  #logicExpr
       // логическое ИЛИ
       | expr OR expr                                                   #logicExpr
       // определение массива
       | array_definition                                               #arrDefExpr
       // объявление массива
       | array_declaration                                              #arrDeclareExpr
       // присваивание
       | assignment                                                     #assignExpr
       ;

puts : PUTS expr;

delim : (SEMICOLON | NEWLINE)+;