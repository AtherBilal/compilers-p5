// File token.c

#ifndef TOKEN_H
#define TOKEN_H

// Enumeration of tokens  
typedef enum tokenID {
	// EOF, Error 
	EOF_tk, ERR_tk,

	//  Identifiers and Integers 
	ID_tk, INT_tk,

    //  Operators 
    EQUAL_tk, LESS_tk, GREAT_tk,
	EQEQ_tk, COLON_tk, PLUS_tk, MINUS_tk,
	ASTERISK_tk, SLASH_tk, NUM_tk, PERIOD_tk, AND_tk,

    // Delimiters 
    OPENPAR_tk, CLOSEPAR_tk, COMMA_tk, OBRACE_tk, CBRACE_tk, 
	SCOLON_tk, OPENBRACKET_tk, CLOSEBRACKET_tk,

	// Keywords 
	BEGIN_tk, END_tk, REPEAT_tk, VOID_tk, VAR_tk,
	RETURN_tk, SCAN_tk, WRITE_tk, PROGRAM_tk,
	IF_tk, THEN_tk, LET_tk
} token_ID;

// Token definition 
typedef struct token{
	token_ID id; 
	char *tkString;
	int line;
} token;
						
#endif 