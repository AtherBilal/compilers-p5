//testTree.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"
#include "testTree.h"

// function for printing the tree in preorder
void printPreorder(node_t *root){

  // if array is empty
	if (root == NULL){
		printf("Error: Array is Empty\n");
		return;
	}
	
	if(root->token.tkString != NULL && root->token2.tkString != NULL){
		printf("%*s%-9s\n", root->level*2," ", concat(concat(root->name, root->token.tkString), root->token2.tkString));
	}
	else if(root->token.tkString != NULL && root->token2.tkString == NULL){
		printf("%*s%-9s\n", root->level*2," ", concat(root->name, root->token.tkString));
	}
	else{
		printf("%*s%-9s\n", root->level*2," ", root->name);
	}

	if (root->child1 != NULL)
		printPreorder(root->child1);
	if (root->child2 != NULL)
		printPreorder(root->child2);
	if (root->child3 != NULL)
		printPreorder(root->child3);
	if (root->child4 != NULL)
		printPreorder(root->child4);
}

//string concatenation function
char *concat(const char *string1, const char *string2){
  char *newString = malloc(snprintf(NULL, 0, "%s %s", string1, string2) + 1);
  if(newString == NULL){
	printf("Concat Failed\n");
	exit(EXIT_FAILURE);
  }
  sprintf(newString, "%s %s", string1, string2);
  return newString;
}