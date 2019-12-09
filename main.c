//main.c
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "parser.h"
#include "node.h"
#include "testTree.h"
#include "staticSemantics.h"
#include "codeGeneration.h"



int main(int argc, char* argv[]){
	node_t *tree = NULL;
	FILE *outputFile;
 
	if (argc > 2){
		printf("Error: Too many arguments.\n");
		exit(EXIT_FAILURE);
	}
   
	if(argc == 1){
		// input file pointer 
		input = stdin;

   		// if stdin open fails
		if (input == NULL){
			fprintf(stderr, "Stdin Error: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}

		// user input
		if (isatty(0)){
			printf("\nCould not find file. \nEnter Input: ");
		}
   
		if(!feof(input)){
			tree = parser();
		}
		printf("Semantic Check Started\n");
		semanticCheck(tree);
		printf("Semantic Check Complete\n");
		printSymbolTable();
  		outputFile = fopen("kb.asm", "w");
		codeGeneration(tree, outputFile);
		writeVariables(outputFile);
		fclose(outputFile);
		fclose(input);
	} else {
		char *fileName = argv[1];
		input = fopen(fileName, "r");

		//  input file failure 
		if (input == NULL){
			fprintf(stderr, "Unable To Open '%s': %s\n", argv[1], strerror(errno));
			exit(EXIT_FAILURE);
		}

		if(!feof(input)) {
			tree = parser();
		}
		printf("Semantic Check Started\n");
		semanticCheck(tree);
		printf("Semantic Check Complete\n");
		char *outExtension = ".asm";
		char *outputFileName = malloc(snprintf(NULL, 0, "%s%s", argv[1], outExtension) + 1);
		sprintf(outputFileName, "%s%s", argv[1], outExtension);
		outputFile = fopen(outputFileName, "w");
		codeGeneration(tree, outputFile);
		writeVariables(outputFile);
		fclose(input);
		fclose(outputFile);

	}
	return 0; 
}
