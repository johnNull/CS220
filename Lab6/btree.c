#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct tnode * tree;

struct tnode {
   tree left;
   tree right;
   int value;
};

tree makeNode(int value);
void freeTree(tree t);

void insertVal(tree t, int value);
void prLNR(tree t);
void prRNL(tree t);
void printTree(tree t, char * prefix);

int main(int argc, char **argv) {
   int i;

   if (argc<2) {
		printf("You must specify at least one argument to %s\n",argv[0]);
		return 1;
	}
	tree root=makeNode(atoi(argv[1]));
   for(i=2; i<argc; i++) {
      int val=atoi(argv[i]);
      insertVal(root,val);
   }
   printf("In LNR order: ");
   prLNR(root);
   printf("\nIn RNL order: ");
   prRNL(root);
   printf("\nGraphic Representation...\n");
   printTree(root,"");
   freeTree(root);
   return 0;
}

tree makeNode(int value) {
   tree t = malloc(sizeof(struct tnode));
   t->value=value;
   t->left=NULL;
   t->right=NULL;
   return t;
}

void freeTree(tree t) {
   assert(t);
   if (t->left) freeTree(t->left);
   if (t->right) freeTree(t->right);
   free(t);
}

void insertVal(tree t, int value) {
	assert(t);
	//if(t == NULL){t = makeNode(value);}
	//else{
		if(value < t->value){
			if(t->left == NULL)
				t->left = makeNode(value);
			else
				insertVal(t->left, value);
		}
		else{
			if(t->right == NULL)
				t->right = makeNode(value);
			else
				insertVal(t->right, value);
		}
	//}
}

void prLNR(tree t) {
	assert(t);
	if(t->left != NULL) {
		prLNR(t->left);
	}
	printf(" %d,", t->value);
	if(t->right != NULL) {
		prLNR(t->right);
	}
   /* Write code to:
   	- prLNR the left sub-tree,
   	- print the value of the node,
   	- prLNR the right sub-tree
   */
}


void prRNL(tree t) {
	if(t->right != NULL)
		prRNL(t->right);
	printf(" %d,", t->value);
	if(t->left != NULL){prRNL(t->left);}
   /* Write code to:
   	- prRNL the right sub-tree,
   	- print the value of the node,
   	- prRNL the left sub-tree
   */
}

void printTree(tree t, char * prefix) {
	//assert(t);
	/* Write code to print a tree graphically */
}

