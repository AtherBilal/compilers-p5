// File codeGeneration.c

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "node.h"
#include "staticSemantics.h"

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

static int tempCounter = 1;
static int labelCounter = 1;
static int count = 0;

void codeGeneration(node_t *node, FILE *outputFile){
    // printf("starting code gen\n");
    if(outputFile == NULL){
        printf("outputFile failed to open");
        exit(EXIT_FAILURE);
    }
    if (node == NULL){
        return;
    }
    if(strcmp(node->name, "<G>") == 0) {
        generateG(node, outputFile);
    } 
    else if (strcmp(node->name, "<A>") == 0) {
        generateA(node, outputFile);
    } 
    else if (strcmp(node->name, "<W>") == 0) {
        generateW(node, outputFile);
    } 
    else if (strcmp(node->name, "<E>") == 0) {
        generateE(node, outputFile);
    } 
    else if (strcmp(node->name, "<I>") == 0) {
        generateI(node, outputFile);
    } 
    else {
        codeGeneration(node->child1, outputFile);
        codeGeneration(node->child2, outputFile);
        codeGeneration(node->child3, outputFile);
        codeGeneration(node->child4, outputFile);
    }
}

void generateA(node_t *node, FILE *outputFile){
    if (strcmp(node->token.tkString, "") != 0){
        fprintf(outputFile, "READ %s\n", node->token.tkString);
    }
}

void generateW(node_t *node, FILE *outputFile){
    char *tempVar = createTempVar();
    generateM(node->child1, outputFile);
    fprintf(outputFile, "STORE %s\n", tempVar);
    fprintf(outputFile, "WRITE %s\n", tempVar);
    count = 0;
}



char *createTempVar(){
    char tempNum[12];
    sprintf(tempNum, "%d", labelCounter);
    labelCounter++;
    char *tempVar = concat("temp", tempNum);
    token tempTk;
    tempTk.tkString = tempVar;
    variables[varCount] = tempTk;
    varCount++;
    return tempVar;
}

char *createLabel(){
    char tempNum[12];
    sprintf(tempNum, "%d", tempCounter);
    tempCounter++;
    char *tempLabel = concat("label", tempNum);
    return tempLabel;
}

char *concat(const char *string1, const char *string2){
  char *newString = malloc(snprintf(NULL, 0, "%s%s", string1, string2) + 1);
  if(newString == NULL){
	printf("Concat Failed\n");
	exit(EXIT_FAILURE);
  }
  sprintf(newString, "%s%s", string1, string2);
  return newString;
}

void generateM(node_t *node, FILE *outputFile){
    char *label;
    if(node == NULL){
        return;
    }

    generateM(node->child1, outputFile);

    if(strcmp(node->name, "<R>") == 0){
        if (count == 0){
            fprintf(outputFile, "LOAD %s\n", node->token.tkString);
        } else {
            fprintf(outputFile, "%s\n", node->token.tkString);
        }
        count++;
    }

    if(strcmp(node->name, "<H>") == 0){
        if (node->token.tkString != NULL && strcmp(node->token.tkString, "&") == 0){
            label = createLabel();
            fprintf(outputFile, "BRPOS %s\n",label);
            fprintf(outputFile, "MULT -1\n");
            fprintf(outputFile, "%s: NOOP\n",label);
        }
    }
    if(strcmp(node->name, "<M>") == 0){
        if (node->token.tkString != NULL && strcmp(node->token.tkString, "+") == 0){
            fprintf(outputFile, "ADD ");
        } else if (node->token.tkString != NULL && strcmp(node->token.tkString, "-") == 0){
            fprintf(outputFile, "SUB ");
        } else if (node->token.tkString != NULL && strcmp(node->token.tkString, "*") == 0){
            fprintf(outputFile, "MULT ");
        } else if (node->token.tkString != NULL && strcmp(node->token.tkString, "/") == 0){
            fprintf(outputFile, "DIV ");
        }
    }
    generateM(node->child2, outputFile);
}

void generateE(node_t *node, FILE *outputFile){
    generateM(node->child1, outputFile);
    fprintf(outputFile, "STORE %s\n", node->token.tkString);
    count = 0;
}

void generateT(node_t *node, FILE *outputFile){
    if(node == NULL){
        return;
    }
    if(strcmp(node->name, "<W>") == 0){
        generateW(node, outputFile);
    } else if (strcmp(node->name, "<A>") == 0){
        generateA(node, outputFile);
    } else if (strcmp(node->name, "<E>") == 0){
        generateE(node, outputFile);
    } else if (strcmp(node->name, "<I>") == 0){
        generateI(node, outputFile);
    } else if (strcmp(node->name, "<G>") == 0){
        generateG(node, outputFile);
    } else {
        generateT(node->child1, outputFile);
        generateT(node->child2, outputFile);
        generateT(node->child3, outputFile);
        generateT(node->child4, outputFile);
    }
}


//Here is the nonterminal, 'I', for if statements
void generateI(node_t *node, FILE *outputFile){
    char *temp;
    char *label;
    char *oper;

    if (node->child3 != NULL){
        generateM(node->child3, outputFile);
        count = 0;
    }

    temp = createTempVar();

    fprintf(outputFile, "STORE %s\n", temp);

    generateM(node->child1, outputFile);

    count = 0;
    
    fprintf(outputFile, "SUB %s\n", temp);

    label = createLabel();

    oper = generateZ(node->child2);

    if(strcmp(oper, ">") == 0){
        fprintf(outputFile, "BRZNEG %s\n", label);
    }
    else if(strcmp(oper, "<") == 0){
        fprintf(outputFile, "BRZPOS %s\n", label);
    }
    else if(strcmp(oper, "=") == 0|| strcmp(oper, "==") == 0 || strcmp(oper, ":") == 0){
        fprintf(outputFile, "BRNEG %s\n", label);
        fprintf(outputFile, "BRPOS %s\n", label);
    } else {
        printf("ERROR");
        exit(0);
    }
    generateT(node->child4, outputFile);
    fprintf(outputFile, "%s : NOOP\n", label);
}

char* generateZ(node_t *node){
    if (node == NULL){
        return "";
    }
    if(strcmp(node->name, "<Z>") == 0){
        return node->token.tkString;
    }
    generateZ(node->child1);
    generateZ(node->child2);
    generateZ(node->child3);
    generateZ(node->child4);
    return ""; 
}

void generateG(node_t *node, FILE *outputFile){
    char *temp;
    char *label1;
    char *label2;
    char *oper;

    label1 = createLabel();
    label2 = createLabel();

    fprintf(outputFile,"%s: NOOP\n", label1);

    generateM(node->child3, outputFile);
    count = 0;

    temp = createTempVar();
    fprintf(outputFile,"STORE %s\n", temp);

    generateM(node->child1, outputFile);
    count = 0;

    fprintf(outputFile,"SUB %s\n", temp);

    oper = generateZ(node->child2);

    if(strcmp(oper, ">") == 0){
        fprintf(outputFile, "BRZNEG %s\n", label2);
    }
    else if(strcmp(oper, "<") == 0){
        fprintf(outputFile, "BRZPOS %s\n", label2);
    }
    else if(strcmp(oper, "=") == 0|| strcmp(oper, "==") == 0 || strcmp(oper, ":") == 0){
        fprintf(outputFile, "BRNEG %s\n", label2);
        fprintf(outputFile, "BRPOS %s\n", label2);
    } else {
        printf("ERROR");
        exit(0);
    }
    generateT(node->child4, outputFile);
    fprintf(outputFile, "BR %s\n", label1);
    fprintf(outputFile, "%s : NOOP\n", label2);
}

// at the end of the file initialize variables
void writeVariables(FILE *outputFile){
    fprintf(outputFile, "STOP\n");
    int i;
    for (i = 0; i < varCount; i++){
        fprintf(outputFile, "%s 0\n", variables[i].tkString);
    }
}

