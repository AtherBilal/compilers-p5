// parser.h

#ifndef PARSER_H
#define PARSER_H

#include "node.h"

extern FILE *input;

//parser function
node_t *parser();

node_t *S();
node_t *B(int level);
node_t *V(int level);
node_t *M(int level);
node_t *H(int level);
node_t *R(int level);
node_t *Q(int level);
node_t *T(int level);
node_t *A(int level);
node_t *W(int level);
node_t *I(int level);
node_t *G(int level);
node_t *E(int level);
node_t *Z(int level);


//new node function
node_t *createNode(char *name, int level);

#endif