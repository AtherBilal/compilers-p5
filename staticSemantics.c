//staticSemantics.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "staticSemantics.h"

token variables[MAXVARSIZE];

//Loop counter var
int i = 0;

//Initialize all variables
void variablesInit(void) {
	for(i = 0; i <= MAXVARSIZE; i++) {
		variables[i].tkString = "";
	}
	return;
}

//variables insert 
void insert(token variable) {
	//Warn too many variables
	if(varCount == MAXVARSIZE){
		printf("Variables are full\n");
		exit(EXIT_FAILURE);
	}
	//Check for variable existence before insertion
	for(i = 0; i < varCount; i++){
		if(strcmp(variables[i].tkString, variable.tkString) == 0){
			printf("%s is already declared on line: %d\n", variable.tkString, variables[i].line);
			exit(EXIT_FAILURE);
		}
	}	
	//insert token to the variables 
	variables[varCount] = variable;
 
	//Increment number of variables
	varCount++;
}

//Make sure variable exists
bool verify(char *var) {
	for(i = 0; i < varCount; i++){
		if(strcmp(variables[i].tkString, var) == 0) {
			return true;
		}
	}
	//Not found
	return false;
}

// checking for variable declariations
void semanticCheck(node_t *node) {
	
	if(node == NULL){
		return;
	}
	//Process starting node  
	if(strcmp(node->name, "<S>") == 0) {
		if(node->child1 != NULL) {
		    semanticCheck(node->child1);
        }
		if(node->child2 != NULL) {
		    semanticCheck(node->child2);
        }
	} else if(strcmp(node->name, "<V>") == 0) {
		//Add to array if it doesn't already exist
        insert(node->token);
		if(node->child1 != NULL) {
            semanticCheck(node->child1);
        }
	} else if (node->token.id == ID_tk && !verify(node->token.tkString)) {
            printf("'%s' Is not defined on Line: %d\n", node->token.tkString, node->token.line);
			exit(EXIT_FAILURE);   
    } else {
		if(node->child1 != NULL) {
	        semanticCheck(node->child1);
        }
		if(node->child2 != NULL) {
		    semanticCheck(node->child2);
        }
		if(node->child3 != NULL) {
		    semanticCheck(node->child3);
        }
		if(node->child4 != NULL) {
		    semanticCheck(node->child4);
        }
	}
	return;
}

// prints all symbols
void printSymbolTable(){
    int i;
    for (i = 0; i < varCount; i++){
        printf("'%s'\n", variables[i].tkString);
    }
}