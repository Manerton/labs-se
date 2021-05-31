lexer grammar TLexer;

// Follows directly after the standard #includes in h + cpp files.
@lexer::postinclude {
/* lexer postinclude section */
#ifndef _WIN32
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
}

Program: Lexeme | Whitespace;
Lexeme: ;
Literal: ;
Special: '(' | ')' | ',' | ';' | '[' | ']' | '`' | '{' | '}';
Whitespace: ;
Whitestuff: ;
Whitechar: ;
Newline: ;
Space: ' ';
Tab: '  ';
Comment: ;
NComment: ;
Any: ;
Graphic: ;
Small: ;
Large: ;
Symbol: ;
Octit: [0-7];
Digit: ;
Hexit: ;
Octal: ;
Decimal: ;
Hex: ;
Integer: ;
Float: ;
Char: ;
String: ;
Boolean: ;
Id: ;
Type: ;
Reserve: ;


ID: LETTER (LETTER | '0'..'9')*;
fragment LETTER : [a-zA-Z\u0080-\u{10FFFF}];

LessThan: '<';
GreaterThan:  '>';
Equal: '=';
And: 'and';

Colon: ':';
Semicolon: ';';
Plus: '+';
Minus: '-';
Star: '*';
OpenPar: '(';
ClosePar: ')';
OpenCurly: '{' -> pushMode(Mode1);
CloseCurly: '}' -> popMode;
QuestionMark: '?';
Comma: ',' -> skip;
Dollar: '$' -> more, mode(Mode1);
Ampersand: '&' -> type(DUMMY);

String: '"' .*? '"';
Foo: {canTestFoo()}? 'foo' {isItFoo()}? { myFooLexerAction(); };
Bar: 'bar' {isItBar()}? { myBarLexerAction(); };
Any: Foo Dot Bar? DotDot Baz;

Comment : '#' ~[\r\n]* '\r'? '\n' -> skip;
WS: [ \t\r\n]+ -> channel(99);

