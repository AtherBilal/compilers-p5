// File scanner.c

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "scanner.h"
#include "token.h"

#define FSACOLUMNS 24
#define FSAROWS 24

token scanner(FILE *inputFile, int *line);
token lookup(int state, char *str);
int findColumn(int currentCharacter);



int FSA[FSAROWS][FSACOLUMNS] = {
	    //  ws   a-z   0-9    =     <     >     :     +     -     *     /     &     #     .    (      )     ,    {     }      ;    [      ]    EOF  A-Z
/*s0*/	{   0,    1,    2,    3,    4,    5,    6,    7,    8,   9,   10,   11,   12,   13,   14,   15,   16,  17,    18,   19,   20,   21,   -1,   -2},
/*s1*/	{  -2,   23,   23, 	 -2, 	-2,	 -2,   -2,   -2,   -2,   -2,   -2,   -2,   -2,  -2,    -2,   -2,   -2,   -2,   -2,   -2,   -2,   -2,   -2,   23},
/*s2*/	{1001, 1001,    2, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001, 1001},
/*s3*/	{1002, 1002, 1002,   22, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002},
/*s4*/	{1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003, 1003},
/*s5*/	{1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004},
/*s6*/	{1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006, 1006},
/*s7*/	{1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007, 1007},
/*s8*/	{1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008, 1008},
/*s9*/	{1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009, 1009},
/*s10*/	{1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010, 1010},
/*s11*/	{1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021, 1021},
/*s12*/	{1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011, 1011},
/*s13*/	{1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012, 1012},
/*s14*/	{1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013, 1013},
/*s15*/	{1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014, 1014},
/*s16*/	{1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015, 1015},
/*s17*/	{1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016, 1016},
/*s18*/	{1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017, 1017},
/*s19*/	{1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018, 1018},
/*s20*/	{1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019, 1019},
/*s21*/	{1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020, 1020},
/*s22*/	{1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005, 1005},
/*s23*/	{1000,   23,  23, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,   23},
};

//special character constructs are stored here
characterMap specialChars[] = {
	{61, 3}, {60, 4}, {62, 5}, {58, 6}, {43, 7},			// = | < | > | : | +
	{45, 8}, {42, 9}, {47, 10}, {38, 11}, {35, 12},    		// - | * | / | & | #
	{46, 13}, {40, 14}, {41, 15}, {44, 16}, {123, 17},		// . | ( | ) | , | {
	{125, 18}, {59, 19}, {91, 20}, {93, 21}					// } | ; | [ | ] 
};

// This array maps the final state to the specified token
stateMap finalStateLookup[] = {
	{-1, EOF_tk},
	{-2, ERR_tk},
	{1000, ID_tk},
	{1001, INT_tk},
	{1002, EQUAL_tk},
	{1003, LESS_tk},
	{1004, GREAT_tk},
	{1005, EQEQ_tk},
	{1006, COLON_tk},
	{1007, PLUS_tk},
	{1008, MINUS_tk},
	{1009, ASTERISK_tk},
	{1010, SLASH_tk},
	{1011, NUM_tk},
	{1012, PERIOD_tk},
	{1013, OPENPAR_tk},
	{1014, CLOSEPAR_tk},
	{1015, COMMA_tk},
	{1016, OBRACE_tk},
	{1017, CBRACE_tk},
	{1018, SCOLON_tk},
	{1019, OPENBRACKET_tk},
	{1020, CLOSEBRACKET_tk},
	{1021, AND_tk}
};

token keywordLookup[] = {
	{BEGIN_tk, "begin", 0}, 
	{END_tk, "end", 0}, 
	{REPEAT_tk, "repeat", 0},
	{IF_tk, "if", 0},
	{THEN_tk, "then", 0},
	{LET_tk, "let", 0},
	{VOID_tk, "void", 0}, 
	{VAR_tk, "var", 0},
	{RETURN_tk, "return", 0}, 
	{SCAN_tk, "scan", 0},
	{WRITE_tk, "write", 0},
	{PROGRAM_tk, "program", 0}
};

// This is the main scanner function
token scanner(FILE *inputFile, int *line) {
	token returnToken;

	int currentCharacter;
	int column;
	int currState = 0;
	int nextState = 0;

	char *currWord = malloc(9);
	char *currentCharacterStr = malloc(2);

	// default token values
	returnToken.tkString = NULL;
	returnToken.id = 999;
	returnToken.line = 999;

	//read from file
	while (currState > -1 && currState < 500){

		currentCharacter = fgetc(inputFile);

		// skip comments
		if (currentCharacter == '%') {
			do {
				currentCharacter = fgetc(inputFile);
			} while (currentCharacter != '%');
			// skip the next % too
			currentCharacter = fgetc(inputFile);
		}
			
		column = findColumn(currentCharacter);
		sprintf(currentCharacterStr, "%c", currentCharacter);

		// find the next state
		nextState = FSA[currState][column];

		
		if (column == -2) {
			returnToken.id = ERR_tk;
			returnToken.line = *line;
			strcpy(currWord, currentCharacterStr);
			printf("\nScanner Error: Bad character: %c.\n", currentCharacter);
			returnToken.tkString = currWord;
			return returnToken;
		}
		
		if (nextState > 999 || nextState == -1 || nextState == -2) {
			// EOF 
			if (nextState == -1) {
				returnToken.id = EOF_tk;
				returnToken.line = *line;
				returnToken.tkString = "EOF";
				return returnToken;
			}
			
			// States over 1000 are non-error states
			else if (nextState > 999) {
				returnToken = lookup(nextState, currWord);
				returnToken.line = *line;
				ungetc(currentCharacter, inputFile);
				return returnToken;
			} else {
				// Handle errors
				ungetc(currentCharacter, inputFile);
				int temp = fgetc(inputFile);
				sprintf(currentCharacterStr, "%c", temp);
				strcat(currWord, currentCharacterStr);
				printf("\nScanner Error: Line: %i. Invalid Character: %c.\n", *line, currentCharacter);
				returnToken.id = ERR_tk;
				returnToken.line = *line;
				returnToken.tkString = currWord;
				return returnToken;
			}
		} else {
			if (!isspace(currentCharacter)){
				strcat(currWord, currentCharacterStr);
			}
			// Enforce 8 character limit
			if (strlen(currWord) > 8){
				printf("Scanner Error: Word length limit is 8 character %s is too long\n", currWord);
				returnToken.id = ERR_tk;
				returnToken.line = *line;
				returnToken.tkString = "Length";
				return returnToken;
			}
			if (currentCharacter == '\n'){
				// increment line count
				(*line)++;
			}
			currState = nextState;
		}
	}
	returnToken.id = ERR_tk;
	returnToken.line = *line;
	returnToken.tkString = "Scanner Error: Failure";
	return returnToken;
}

// Map final word to token
token lookup(int state, char *str) {
	token returnToken;
	int i;
	for (i = 0; i < TOKENS; i++) {
		if (finalStateLookup[i].state == state) {
			returnToken.id = finalStateLookup[i].tkEnum;
			returnToken.tkString = str;
			returnToken.line = 999; 
		}
	}
	// check if word is a key word
	if (state == 1000) {
		for (i = 0; i < KEYWORDS; i++) {
			if (strcmp(keywordLookup[i].tkString, str) == 0) {
				returnToken.id = keywordLookup[i].id;
				returnToken.tkString = keywordLookup[i].tkString;
				returnToken.line = 999; 
			}
		}
	}
	return returnToken;
}

// point to correct row in FSA
int findColumn(int currentCharacter) {     
	if (isalpha(currentCharacter)) {
		if(isupper(currentCharacter)){
			return 23;
		} else {
			return 1;
		}
	} else if (isdigit(currentCharacter)) { 
		return 2;
	} else if (isspace(currentCharacter)) {  
		return 0;
	} else if (currentCharacter == EOF) {   
		return 22;
	} else if(currentCharacter != EOF || !isspace(currentCharacter) || !isdigit(currentCharacter) || !isalpha(currentCharacter)){ 
		int j;
		for (j = 0; j < SPECIALCHARS; j++) {        
			if(specialChars[j].input == currentCharacter){
				return specialChars[j].column;
			}
		}
	}
	return -2;
}
