parser grammar TParser;

options {
	tokenVocab = TLexer;
}

// Follows directly after the standard #includes in h + cpp files.
@parser::postinclude {
/* parser postinclude section */
#ifndef _WIN32
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
}

// Actual grammar start.
main: stat+ EOF;

stat: expr Equal expr Semicolon
    | expr Semicolon
;

expr: expr Star expr
    | expr Plus expr
    | OpenPar expr ClosePar
    | <assoc = right> expr QuestionMark expr Colon expr
    | <assoc = right> expr Equal expr
    | id
    | flowControl
    | INT
    | String
;

flowControl:
	Return expr # Return
	| Continue # Continue
;

id: ID;
array : OpenCurly el += INT (Comma el += INT)* CloseCurly;
idarray : OpenCurly element += id (Comma element += id)* CloseCurly;
any: t = .;
