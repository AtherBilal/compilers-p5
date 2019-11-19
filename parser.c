//parser.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "scanner.h"
#include "token.h"

FILE *input = NULL;

//current token
token tk;

int lineNum = 1;

// names of tokens stored here
static const char *tokenName[] ={
	[EOF_tk] = "EOF_tk", 
	[ERR_tk] = "ERR_tk", 
	[ID_tk] = "ID_tk", // identifier
	[INT_tk] = "INT_tk", // integer
  	[EQUAL_tk] = "EQUAL_tk", // =
	[LESS_tk] = "LESS_tk", // <
	[GREAT_tk] = "GREAT_tk", // >
	[EQEQ_tk] = "EQEQ_tk",  // == 
  	[COLON_tk] = "COLON_tk", // :
	[PLUS_tk] = "PLUS_tk",  // +
	[MINUS_tk] = "MINUS_tk",  // -
	[ASTERISK_tk] = "ASTERISK_tk", // * 
	[SLASH_tk] = "SLASH_tk", // /
	[NUM_tk] = "NUM_tk", // #
	[PERIOD_tk] = "PERIOD_tk", // .
	[AND_tk] = "AND_tk", // &
  	[OPENPAR_tk] = "OPENPAR_tk", // (
	[CLOSEPAR_tk] = "CLOSEPAR_tk",  // )
	[COMMA_tk] = "COMMA_tk",  // ,
	[OBRACE_tk] = "OBRACE_tk", // {
	[CBRACE_tk] = "CBRACE_tk", // }
	[SCOLON_tk] = "SCOLON_tk", // ;
	[OPENBRACKET_tk] = "OPENBRACKET_tk", // [
	[CLOSEBRACKET_tk] = "CLOSEBRACKET_tk", // ]
	[BEGIN_tk] = "BEGIN_tk", 
	[END_tk] = "END_tk", 
	[REPEAT_tk] = "REPEAT_tk",  
	[VOID_tk] = "VOID_tk", 
	[VAR_tk] = "VAR_tk", 
	[RETURN_tk] = "RETURN_tk", 
	[SCAN_tk] = "SCAN_tk", 
	[WRITE_tk] = "OUTPUT_tk", 
	[PROGRAM_tk] = "PROGRAM_tk",
  	[IF_tk] = "IF_tk",
  	[THEN_tk] = "THEN_tk",
  	[LET_tk] = "LET_tk"
 }; 
  
 //parser function
 node_t *parser() {
	node_t *node;
	tk = scanner(input, &lineNum);
	node = S();

	if(tk.id != EOF_tk){
		printf("Error: Parse Failed. Expected EOF_tk -- %s Returned at Line %d\n", tokenName[tk.id], lineNum);
		exit(EXIT_FAILURE);
	}		
	return node;
}

//All programs start with nonterminal <S>
//<S> -> program <V> <B>
node_t *S(){

	int level = 0;
	node_t *node = createNode("<S>", level);
	if(tk.id == PROGRAM_tk){									
		tk = scanner(input, &lineNum);
		node->child1 = V(level);
		node->child2 = B(level);
		return node;
	}
	else{
		printf("Error: Invalid Token, Expected PROGRAM_tk -- %s Received At Line %d\n", tokenName[tk.id], lineNum);
		exit(EXIT_FAILURE);
	}
}

// <B> -> begin <V> <Q> end
node_t *B(int level){
	level++;
	node_t *node = createNode("<B>", level);
	if(tk.id == BEGIN_tk){											
		tk = scanner(input, &lineNum);
		node->child1 = V(level);
		node->child2 = Q(level);
		if (tk.id == END_tk){
			tk = scanner(input, &lineNum);
			return node;
		}
		else{
			printf("Error: Invalid Token, Expected END_tk -- %s Received At Line %d\n", tokenName[tk.id], lineNum);
			exit(EXIT_FAILURE);
		}
	}
	else{
		printf("Error: Invalid Token, Expected BEGIN_tk -- %s Received At Line %d\n", tokenName[tk.id], lineNum);
		exit(EXIT_FAILURE);
	}
}


// <V> -> empty | var identifier . <V> 
node_t *V(int level)
{
	level++;
	node_t *node = createNode("<V>", level);
	if(tk.id == VAR_tk) {
		tk = scanner(input, &lineNum);
		if(tk.id == ID_tk) {
			node->token = tk;
			tk = scanner(input, &lineNum);

			if(tk.id == PERIOD_tk) {
				node->token2 = tk;
				tk = scanner(input, &lineNum);
				node->child1 = V(level);
				return node;
			} else {
				printf("Error: Invalid Token, Expected PERIOD_tk -- %s Received At Line %d\n", tokenName[tk.id], lineNum);
				exit(EXIT_FAILURE);
			}
		} else {
			printf("Error: Invalid Token, Expected ID_tk -- %s Received At Line %d\n", tokenName[tk.id], lineNum);
			exit(EXIT_FAILURE);
		}
	}
	return NULL;															
}

// <M> -> <H> + <M> | <H> - <M> | <H> / <M> | <H> * <M> | <H>
node_t *M(int level)
{                    
  if (tk.id == AND_tk || tk.id == ID_tk || tk.id == INT_tk){
	level++;
	node_t *node = createNode("<M>", level);
	node->child1 = H(level);

	if(tk.id == PLUS_tk) {
		node->token =tk;
		tk = scanner(input, &lineNum);
		node->child2 = M(level);
		return node;
	} else if(tk.id == MINUS_tk) {
		node->token = tk;
		tk = scanner(input, &lineNum);
		node->child2 = M(level);
		return node;
	} else if(tk.id == SLASH_tk) {
		node->token =tk;
		tk = scanner(input, &lineNum);
		node ->child2 = M(level);
		return node;
	} else if(tk.id == ASTERISK_tk) {
		node->token = tk;
		tk = scanner(input, &lineNum);
		node->child2 = M(level);
		return node;
	} else {
		return node;
	}
  } else {
	printf("Error: Expected ID_tk, INT_tk or AND_tk -- %s Returned at Line %d\n", tokenName[tk.id], lineNum);
	exit(EXIT_FAILURE);
  }
}

// <H> -> & <R> | <R>
node_t *H (int level) {
  level++;
  node_t *node = createNode("<H>", level);
  if(tk.id == AND_tk) {
    node->token = tk;
	tk =scanner(input, &lineNum);
    node->child1 = R(level);
    return node;
  } else {
    node->child1 = R(level);
    return node;
  }
}

// <R> -> identifier | number
node_t *R(int level) {
	level++;
	node_t *node = createNode("<R>", level);
	if(tk.id == ID_tk) {
		node->token = tk;
		tk = scanner(input, &lineNum);
		return node;   
	} else if(tk.id == INT_tk){
		node->token = tk;
		tk = scanner(input, &lineNum);
		return node;
	} else {
		printf("Error: Expected ID_tk or INT_tk -- %s Returned at Line %d\n", tokenName[tk.id], lineNum);
		exit(EXIT_FAILURE);
	}
}

// <Q> -> <T> # <Q> | empty
node_t *Q(int level){
	if(tk.id == SCAN_tk || tk.id == WRITE_tk || tk.id == BEGIN_tk || tk.id == IF_tk || tk.id == REPEAT_tk || tk.id == LET_tk){
		level++;
		node_t *node = createNode("<Q>", level);
		node->child1 = T(level);
		if(tk.id == NUM_tk){
			node->token = tk;
			tk = scanner(input, &lineNum);
			node->child2 = Q(level);
			return node;
		} else {
			printf("Error: Expected NUM_tk -- %s Returned at Line %d\n", tokenName[tk.id], lineNum);
			exit(EXIT_FAILURE);
		}
	} else {
		return NULL;
	}
}

// <T> -> <A> , | <W> , | <B> | <I> , | <G> , | <E> ,
node_t *T(int level) {
  	level++;
	node_t *node = createNode("<T>", level);
	if(tk.id == SCAN_tk){
		node->child1 = A(level);
		if(tk.id == COMMA_tk){
			node->token = tk;
			tk = scanner(input, &lineNum);
			return node;
		} else {
			printf("Parser Error: Expected COMMA_tk -- %s Returned at Line %d\n", tokenName[tk.id], lineNum);
			exit(EXIT_FAILURE);
		}
	} else if(tk.id == WRITE_tk){									
		node->child1 = W(level);
		if(tk.id == COMMA_tk){
			node->token = tk;
			tk = scanner(input, &lineNum);
			return node;
		} else {
			printf("Parser Error: Expected COMMA_tk -- %s Returned at Line %d\n", tokenName[tk.id], lineNum);
			exit(EXIT_FAILURE);
		}
	} else if(tk.id == BEGIN_tk) {									
		node->child1 = B(level);
		return node;
	} else if(tk.id == IF_tk) {								   
		node->child1 = I(level);
		if(tk.id == COMMA_tk) {
			node->token = tk;
			tk = scanner(input, &lineNum);
			return node;
		} else {
			printf("Parser Error: Expected COMMA_tk -- %s Returned at Line %d\n", tokenName[tk.id], lineNum);
			exit(EXIT_FAILURE);
		}
	} else if(tk.id == REPEAT_tk) {									
		node->child1 = G(level);
		if(tk.id == COMMA_tk){
			node->token = tk;
			tk = scanner(input, &lineNum);
			return node;
		} else {
			printf("Parser Error: Expected COMMA_tk -- %s Returned at Line %d\n", tokenName[tk.id], lineNum);
			exit(EXIT_FAILURE);
		}
	} else if(tk.id == LET_tk) {
		node->child1 = E(level);
		if(tk.id == COMMA_tk){
			node->token = tk;
			tk = scanner(input, &lineNum);
			return node;
		} else {
			printf("Parser Error: Expected COMMA_tk -- %s Returned at Line %d\n", tokenName[tk.id], lineNum);
			exit(EXIT_FAILURE);
		}
	} else {
		printf("Parser Error: Expected SCAN_tk, WRITE_tk, BEGIN_tk, IF_tk, REPEAT_tk or ID_tk -- %s Returned at Line %d\n", tokenName[tk.id], lineNum);
		exit(EXIT_FAILURE);
	}
}

// <A> -> scan identifier | scan number
node_t *A(int level) {
  level++;
  node_t *node = createNode("<A>", level);
  if (tk.id == SCAN_tk){
	tk = scanner(input, &lineNum);
	if(tk.id == ID_tk){
		node->token = tk;
		tk = scanner(input, &lineNum);
		return node;
	} else if (tk.id == INT_tk){
		node->token = tk;
		tk = scanner(input, &lineNum);
		return node;
	} else {
		printf("Parser Error: Expected INDENTIFIER_tk, INT_tk -- %s Returned at Line %d\n", tokenName[tk.id], lineNum);
		exit(EXIT_FAILURE);
	}
  } else {
		printf("Parser Error: Expected SCAN_tk -- %s Returned at Line %d\n", tokenName[tk.id], lineNum);
		exit(EXIT_FAILURE);	  
  }
}

// <W> -> write <M>
node_t *W (int level){
	if (tk.id == WRITE_tk){ // WRITE
		level++;
  		node_t *node = createNode("<W>", level);
		tk = scanner(input, &lineNum);
    	node->child1 = M(level);
		return node;
	} else {
		printf("Parser Error: Expected WRITE_tk -- %s Returned at Line %d\n", tokenName[tk.id], lineNum);
		exit(EXIT_FAILURE);
	}
}

// <I> -> if [ <M> <Z> <M> ] <T>
node_t *I (int level) {

  if(tk.id == IF_tk){
		tk = scanner(input, &lineNum);
		if(tk.id == OPENBRACKET_tk){
			level++;
			node_t *node = createNode("<I>", level);
			node->token = tk;
			tk = scanner(input, &lineNum);
			node->child1 = M(level);
			node->child2 = Z(level);
			node->child3 = M(level);
			if(tk.id == CLOSEBRACKET_tk){
				node->token2 = tk;
				tk = scanner(input, &lineNum);
				node->child4 = T(level);
				return node;
			} else {
				printf("Parser Error: Expected CLOSEBRACKET_tk -- %s Returned at Line %d\n", tokenName[tk.id], lineNum);
				exit(EXIT_FAILURE);
			}
		} else {
			printf("Parser Error: Expected OPENBRACKET_tk -- %s Returned at Line %d\n", tokenName[tk.id], lineNum);
			exit(EXIT_FAILURE);
		}
	} else {
		printf("Parser Error: Expected IF_tk -- %s Returned at Line %d\n", tokenName[tk.id], lineNum);
		exit(EXIT_FAILURE);
	}

}

// <G> -> repeat [ <M> <Z> <M> ] <T>
node_t *G (int level) {
  if(tk.id == REPEAT_tk) {
		tk = scanner(input, &lineNum);
		if(tk.id == OPENBRACKET_tk){
			level++;
			node_t *node = createNode("<G>", level);
			node->token = tk;
			tk = scanner(input, &lineNum);
			node->child1 = M(level);
			node->child2 = Z(level);
			node->child3 = M(level);
			if(tk.id == CLOSEBRACKET_tk){
				node->token2 = tk;
				tk = scanner(input, &lineNum);
				node->child4 = T(level);
				return node;
			} else {
				printf("Parser Error: Expected CLOSEBRACKET_tk -- %s Returned at Line %d\n", tokenName[tk.id], lineNum);
				exit(EXIT_FAILURE);
			}
		} else {
			printf("Parser Error: Expected OPENBRACKET_tk -- %s Returned at Line %d\n", tokenName[tk.id], lineNum);
			exit(EXIT_FAILURE);
		}
	} else {
		printf("Parser Error: Expected REPEAT_tk -- %s Returned at Line %d\n", tokenName[tk.id], lineNum);
		exit(EXIT_FAILURE);
	}
}

// <E> -> let identifier : <M>
node_t *E (int level) {
	level++;
	node_t *node = createNode("<E>", level);
	if(tk.id == LET_tk) {
		tk = scanner(input, &lineNum);
		if(tk.id == ID_tk) {
			node->token = tk;
			tk = scanner(input, &lineNum);
        	if(tk.id == COLON_tk) {
				node->token2 =tk;
				tk = scanner(input, &lineNum);
				node->child1 = M(level);
				return node;
			} else {
				printf("Error: Invalid Token, Expected COLON_tk -- %s Received At Line %d\n", tokenName[tk.id], lineNum);
				exit(EXIT_FAILURE);
			}
		} else {
			printf("Error: Invalid Token, Expected ID_tk -- %s Received At Line %d\n", tokenName[tk.id], lineNum);
		  	exit(EXIT_FAILURE);
    	}
	} else {
		printf("Error: Invalid Token, Expected LET_tk -- %s Received At Line %d\n", tokenName[tk.id], lineNum);
		exit(EXIT_FAILURE);
	} 
}

// <Z> -> < | > | : | = | = =
node_t *Z(int level) {
	level++;
	node_t *node = createNode("<Z>", level);
	if(tk.id == LESS_tk){					
		node->token = tk;
		tk = scanner(input, &lineNum);
		return node;
	} else if(tk.id == GREAT_tk){			
		node->token = tk;
		tk = scanner(input, &lineNum);
		return node;
	} else if(tk.id == EQEQ_tk){			
		node->token = tk;
		tk = scanner(input, &lineNum);
		return node;
	} else if(tk.id == EQUAL_tk){			
		node->token = tk;
		tk = scanner(input, &lineNum);
		return node;
	} else if (tk.id == COLON_tk){			
		node->token = tk;
		tk = scanner(input, &lineNum);
		return node;
	} else {
		printf("Parser Error: Expected LESS_tk, GREAT_tk, EQEQ_tk, COLON_tk or EQUAL_tk-- %s Returned at Line %d\n", tokenName[tk.id], lineNum);
		exit(EXIT_FAILURE);
	}
}

//createNode function 
node_t *createNode(char *name, int level){
	node_t *newNode;
	newNode = malloc(sizeof(node_t));

	//check for memory failure
	if(newNode == NULL){
		printf("ERROR: createNode function\n");
		exit(EXIT_FAILURE);
	}
	strcpy(newNode->name, name);
	newNode->level = level;
	newNode->child1 = NULL;
	newNode->child2 = NULL;
	newNode->child3 = NULL;
	newNode->child4 = NULL;
	return newNode;
}
