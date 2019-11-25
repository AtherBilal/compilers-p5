//staticSemantics.h
#ifndef SEMANTICS_H
#define SEMANTICS_H

#define MAXVARSIZE 100
		
// needed for bools
#include <stdbool.h>

#include "token.h"
#include "node.h"


void variablesInit(void);
void insert(token variable);
bool verify(char *var);
void semanticCheck(node_t *node);
void printSymbolTable();

#endif 
