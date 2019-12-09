//codeGeneration.h
#ifndef CODEGENERATION_H
#define CODEGENERATION_H

void codeGeneration(node_t *node, FILE *outputFile);
void generateG(node_t *node, FILE *outputFile);
void generateA(node_t *node, FILE *outputFile);
void generateI(node_t *node, FILE *outputFile);
void generateW(node_t *node, FILE *outputFile);
void generateE(node_t *node, FILE *outputFile);
void generateT(node_t *node, FILE *outputFile);
void generateM(node_t *node, FILE *outputFile);
void writeVariables(FILE *outputFile);

char *generateZ(node_t *node);
char *createLabel();
char *createTempVar();
char *concat(const char *string1, const char *string2);


#endif 
