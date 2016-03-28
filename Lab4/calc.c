#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

/* ---------------------------------------------------------------------------------------------

Grammar...

	exp_list :=
		statement |
		statement ; exp_list

	statement :=
		expression |
		assignment |
		while

	assignment :=
		set variable expression

	while :=
		while expression expression

	expression :=
		{ exp_list } |
		term + expression |
		term - expression |
		term

	term :=
		factor * term |
		factor / term |
		factor

	factor :=
		( expression ) |
		integer |
		variable

------------------------------------------------------------------------------------------------ */

/* ---------------------------------------------------------------------------------------------
		Global Declarations
------------------------------------------------------------------------------------------------ */

enum typeEnum { // A token must be one of these types
		NOTHING,
		INTEGER, VARIABLE,
		LPAREN, RPAREN,
		PLUS, MULTIPLY, DIVIDE, MINUS,
		WHILE,
		LCURLY, 	RCURLY,
		SET,
		EOS  // End of Statement (semi-colon)
};

typedef struct {
	enum typeEnum type;
	int location;
	int length;
	long int value;
	char *name;
} token;

struct treeStruct {
	struct treeStruct * left;
	token * action;
	struct treeStruct * right;
};

typedef struct treeStruct tree;

struct {
	char * vname;
	long int vvalue;
	bool valset;
} varSpace[50];

/* ---------------------------------------------------------------------------------------------
		Function Declarations
------------------------------------------------------------------------------------------------ */
token * nextToken(token *current,char *buffer,bool delPrev);
	token * newToken();
	void freeToken(token *t);
	void setValue(token *t,long val);
		void setVar(char * name,long int val);
	void setName(token *t,char *name);
	void tokenError(char * reason,char *exp,token *t);

tree * getExpList(token **tokenPtr,char *buffer);
	tree * getStatement(token **tokenPtr,char *buffer);
		tree * getAssignment(token **tokenPtr,char *buffer);
			tree * getExpression(token ** tokenPtr,char *buffer);
				tree * getTerm(token ** tokenPtr,char *buffer);
					tree * getFactor(token ** tokenPtr,char *buffer);
		tree * getWhile(token **tokenPtr,char *buffer);
	tree * newTree();
	void freeTree(tree * t);

long int evalTree(tree *t,bool debug);
	long getValue(token *t);
		long int getVar(char *name);

/*------------ Unused Utility Functions -------------------*/
void printToken(token * token);
void printTree(tree * t,char * prefix);

int main(int argc,char **argv) {

	token * currentToken;
	char buffer[4096];
	tree * t;

	bool debug=false;

	buffer[0]='\0';

	if (argc < 2) {
		printf("Invoke as %s \"expression\"\n",argv[0]);
		return 1;
	}

	strcpy(buffer,argv[1]);

	currentToken=nextToken(NULL,buffer,false);
	t=getExpList(&currentToken,buffer);
	if (currentToken != NULL) {
		tokenError("Syntax error... data left after expression parsed",buffer,currentToken);
		buffer[currentToken->location]=0x00;
	}

	printf("%s = %ld\n",buffer,evalTree(t,debug));
	freeTree(t);
	return 0;
}

token * nextToken(token *current,char *buffer,bool delPrev) {
	token * result=newToken();

	if (current!=NULL) {
		result->location=current->location+current->length;
		if (delPrev) freeToken(current);
	}

	while(result->length==0) {
		if (result->location >= strlen(buffer)) {
			free(result);
			return NULL;
		}
		switch(buffer[result->location]) {
			case ' ':
			case '\t':
				result->location=result->location+1;
				continue;
			case '+':
				result->type=PLUS;
				result->length=1;
				break;
			case '-':
				result->type=MINUS;
				result->length=1;
				break;
			case '*':
				result->type=MULTIPLY;
				result->length=1;
				break;
			case '/':
				result->type=DIVIDE;
				result->length=1;
				break;
			case ';' :
				result->type=EOS;
				result->length=1;
				break;
			case '(':
				result->type=LPAREN;
				result->length=1;
				break;
			case ')':
				result->type=RPAREN;
				result->length=1;
				break;
			case '{':
				result->type=LCURLY;
				result->length=1;
				break;
			case '}' :
				result->type=RCURLY;
				result->length=1;
				break;

			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				result->type=INTEGER;
				char *endptr;
				setValue(result,strtol(buffer+result->location,&endptr,10));
				result->length=(endptr-buffer)-result->location;
				break;

			case 's':
				if (0==strncmp(buffer+result->location,"set",3)) {
					if ( (strlen(buffer)<=result->location+3) ||
					     (buffer[result->location+3]==' ')   ) {
						result->type=SET;
						result->length=3;
						break;
					}
				}
				int ei=0;
				char varName[20];
				while(isalpha((int)buffer[result->location+ei])) {
					varName[ei]=buffer[result->location+ei];
					if (ei>=strlen(buffer)) break;
					ei++;
					varName[ei]='\0';
				}
				result->type=VARIABLE;
				setName(result,varName);
				result->length=ei;
				break;

			case 'w':
				if (0==strncmp(buffer+result->location,"while",5)) {
					if ( (strlen(buffer)<=result->location+5) ||
					     (buffer[result->location+5]==' ') ||
					     (buffer[result->location+5]=='(') ) {
							result->type=WHILE;
							result->length=5;
							break;
					}
				}
				ei=0;
				while(isalpha((int)buffer[result->location+ei])) {
					varName[ei]=buffer[result->location+ei];
					if (ei>=strlen(buffer)) break;
					ei++;
					varName[ei]='\0';
				}
				result->type=VARIABLE;
				setName(result,varName);
				result->length=ei;
				break;

			default:

				if (isalpha((int)buffer[result->location])) {
					ei=0;
					while(isalpha((int)buffer[result->location+ei])) {
						varName[ei]=buffer[result->location+ei];
						if (ei>=strlen(buffer)) break;
						ei++;
						varName[ei]='\0';
					}
					result->type=VARIABLE;
					setName(result,varName);
					result->length=ei;
					break;
				}

				tokenError("Unrecognized token in expression",buffer,result);
				free(result);
				exit(1);
		}
	}
	return result;
}

token * newToken() {
	token * result=(token *)malloc(sizeof(token));
	result->type=NOTHING;
	result->location=0;
	result->length=0;
	result->value=0;
	result->name=NULL;
	return result;
}

void freeToken(token *t) {
	assert(t!=NULL);
	if (t->type==VARIABLE) free(t->name);
	free(t);
}

void setValue(token *t,long val) {
	assert(t!=NULL);
	if (t->type==VARIABLE) setVar(t->name,val);
	else t->value=val;
}

void setVar(char * name,long int val) {
	int i=0;
	for(i=0; i<50; i++) {
		if (varSpace[i].vname==NULL) {
			varSpace[i].vname=name;
			break;
		}
		if (0==strcmp(varSpace[i].vname,name)) break;
	}
	assert(i<50);
	varSpace[i].vvalue=val;
	varSpace[i].valset=true;
}

void setName(token *t, char * name) {
	assert(t!=NULL);
	if (name==NULL) t->name=NULL;
	else t->name=strdup(name);
}

void tokenError(char * reason,char *exp,token *t) {
	printf("Token error : %s\n",reason);
	printf("  Expression: %s\n",exp);
	if (t==NULL) return;
	char buffer[100];
	memset(buffer,' ',t->location);
	buffer[t->location]=0;
	printf("  Error     : %s^\n",buffer);
}

tree * getExpList(token **tokenPtr,char *buffer) {
	assert((*tokenPtr)!=NULL);
	tree * exp=getStatement(tokenPtr,buffer);
	if ((*tokenPtr)==NULL) return exp; // Single expression only
	if ((*tokenPtr)->type==RCURLY) return exp; // Expression lists closed by rcurly
	if ((*tokenPtr)->type==EOS) {
		tree* result=newTree();
		result->left=exp;
		result->action=(*tokenPtr);
		(*tokenPtr)=nextToken((*tokenPtr),buffer,false);
		if (((*tokenPtr)==NULL) || (*tokenPtr)->type==LCURLY) { // Semicolon at end of buffer ignored
			result->left=NULL;
			freeTree(result);
			return exp;
		}
		result->right=getExpList(tokenPtr,buffer);
		return result;
	}
	printf("Syntax error: unexpected tokens after expression list\n");
	exit(1);
}



tree * getStatement(token **tokenPtr,char *buffer) {
	assert((*tokenPtr)!=NULL);
	if ((*tokenPtr)->type==LCURLY) {
		(*tokenPtr)=nextToken((*tokenPtr),buffer,false);
		tree * exp = getExpList(tokenPtr,buffer);
		if (((*tokenPtr)==NULL) || (*tokenPtr)->type!=RCURLY) {
			printf("Syntax error... unbalanced curly braces... not enough right\n");
			exit(1);
		}
		(*tokenPtr)=nextToken((*tokenPtr),buffer,false);
		return exp;
	}
	tree * asg=getAssignment(tokenPtr,buffer);
	if (asg!=NULL) return asg;
	asg = getWhile(tokenPtr,buffer);
	if (asg != NULL) return asg;
	tree * exp=getExpression(tokenPtr,buffer);
	if ((*tokenPtr)==NULL) return exp;
	if ((*tokenPtr)->type==EOS) return exp;
	printf("Syntax error... expected assignment or expression... found neither\n");
	exit(1);
}

tree * getAssignment(token **tokenPtr,char *buffer) {
	assert((*tokenPtr)!=NULL);
	if ((*tokenPtr)->type!=SET) return NULL;
	tree * result=newTree();
	result->action=(*tokenPtr);
	(*tokenPtr)=nextToken((*tokenPtr),buffer,false);
	if ((*tokenPtr)->type!=VARIABLE) {
		printf("Syntax error... expected variable after set\n");
		exit(1);
	}
	tree * variable=newTree();
	variable->action=(*tokenPtr);
	result->left=variable;
	(*tokenPtr)=nextToken((*tokenPtr),buffer,false);
	result->right=getExpression(tokenPtr,buffer);

	return result;
}

tree * getExpression(token ** tokenPtr,char *buffer) {
	assert((*tokenPtr)!=NULL);
	tree * term1=getTerm(tokenPtr,buffer);
	if ((*tokenPtr)==NULL) return term1;
	if ((*tokenPtr)->type==PLUS || (*tokenPtr)->type==MINUS ) {
		tree* result=newTree();
		result->left=term1;
		result->action=(*tokenPtr);
		(*tokenPtr)=nextToken((*tokenPtr),buffer,false);
		if ((*tokenPtr)==NULL) {
			printf("Syntax error... incomplete expression, expected another term");
			exit(1);
		}
		result->right=getExpression(tokenPtr,buffer);
		return result;
	}
	return term1;
}

tree * getTerm(token ** tokenPtr,char *buffer) {
	assert((*tokenPtr) != NULL);
	tree * fact1=getFactor(tokenPtr,buffer);
	if ((*tokenPtr)==NULL) return fact1;
	if ((*tokenPtr)->type==MULTIPLY || (*tokenPtr)->type==DIVIDE) {
		tree* result=newTree();
		result->left=fact1;
		result->action=(*tokenPtr);
		(*tokenPtr)=nextToken((*tokenPtr),buffer,false);
		if ((*tokenPtr)==NULL) {
			printf("Syntax error... incomplete expression, expected another factor");
			exit(1);
		}
		result->right=getTerm(tokenPtr,buffer);
		return result;
	}
	return fact1;
}

tree * getFactor(token ** tokenPtr,char *buffer) {
	assert((*tokenPtr) != NULL);
	if ((*tokenPtr)->type==LPAREN) {
		(*tokenPtr)=nextToken((*tokenPtr),buffer,true);
		if ((*tokenPtr)==NULL) {
			printf("Syntax error... incomplete expression, expected an expression after open parens\n");
			exit(1);
		}
		tree * result=getExpression(tokenPtr,buffer);
		if ((*tokenPtr)->type != RPAREN) {
			tokenError("Syntax error... expected closing parenthesis after expression",buffer,(*tokenPtr));
			exit(1);
		}
		(*tokenPtr)=nextToken((*tokenPtr),buffer,true);
		return result;
	}

	if ((*tokenPtr)->type==INTEGER || (*tokenPtr)->type==VARIABLE ) {
		tree * result=newTree();
		result->action=(*tokenPtr);
		(*tokenPtr)=nextToken((*tokenPtr),buffer,false);
		return result;
	}
	printf("Syntax error... expected a factor\n");
	exit(1);
}

tree * getWhile(token **tokenPtr,char *buffer) {
	assert((*tokenPtr)!=NULL);
	if ((*tokenPtr)->type!=WHILE) return NULL;
	tree * result=newTree();
	result->action=(*tokenPtr);
	if ((*tokenPtr)!=NULL) (*tokenPtr)=nextToken((*tokenPtr),buffer,false);
	result->left=getExpression(tokenPtr,buffer);
	if ((*tokenPtr)==NULL) {
		printf("Syntax error... expected expression + statement  after while\n");
		exit(1);
	}
	result->right=getStatement(tokenPtr,buffer);
	return result;
}

tree * newTree() {
	tree* result=(tree *)malloc(sizeof(tree));
	result->left=NULL;
	result->action=NULL;
	result->right=NULL;
	return result;
}

void freeTree(tree * t) {
	if (t->left != NULL) freeTree(t->left);
	if (t->right != NULL) freeTree(t->right);
	if (t->action != NULL) freeToken(t->action);
	free(t);
}

long int evalTree(tree *t,bool debug) {
	if (t->action->type==INTEGER) return getValue(t->action);
	if (t->action->type==VARIABLE) return getValue(t->action);
	if (t->action->type==EOS) {
		if (debug) printf("Statement result: %ld\n",evalTree(t->left,debug));
		else evalTree(t->left,debug);
		return evalTree(t->right,debug);
	}
	if (t->action->type==SET) {
		token * var=t->left->action;
		long int newval= evalTree(t->right,debug);
		if (debug) printf("Set %s=%ld\n",var->name,newval);
		setValue(var,newval);
		return newval;
	}
	if (t->action->type==WHILE) {
		tree * cond = t->left;
		tree * body = t->right;
		long int cval,bval=0;
		cval=evalTree(cond,debug);
		while(cval>0) {
			bval=evalTree(body,debug);
			if (debug) printf("While %ld : %ld\n",cval,bval);
			cval=evalTree(cond,debug);
		}
		if (debug) printf("While 0 ... final value is %ld\n",bval);
		return bval;
	}
	long int left=evalTree(t->left,debug);
	long int right=evalTree(t->right,debug);
	switch (t->action->type) {
		case PLUS:
			if (debug) printf("%ld + %ld = %ld\n",left,right,left+right);
			return left+right;

		case MINUS:
			if (debug) printf("%ld - %ld = %ld\n",left,right,left-right);
			return left-right;

		case MULTIPLY:
			if (debug) printf("%ld * %ld = %ld\n",left,right,left*right);
			return left*right;

		case DIVIDE:
			if (debug) printf("%ld / %ld = %ld\n",left,right,left/right);
			return left/right;

		default:
			printf("Unrecognized action for subtree Token: ");
			printToken(t->action);
	}
	return 0;
}

long getValue(token *t) {
	assert(t!=NULL);
	if (t->type==VARIABLE) {
		return getVar(t->name);
	}
	return t->value;
}

long int getVar(char *name) {
	int i;
	for(i=0;i<50;i++) {
		if (!varSpace[i].vname) {
			printf("Variable %s has never been assigned a value\n",name);
			exit(1);
		}
		if (0==strcmp(varSpace[i].vname,name)) {
			if (varSpace[i].valset) return varSpace[i].vvalue;
			printf("Variable %s has never been assigned a value\n",name);
			exit(1);
		}
	}
	assert(0);
}

void printToken(token * t) {
	char ttype[20];

	if (t==NULL) {
		printf(" NULL token\n");
		return;
	}
	switch(t->type) {
		case INTEGER : strcpy(ttype,"Integer"); break;
		case LPAREN  : strcpy(ttype,"Left Paren"); break;
		case RPAREN  : strcpy(ttype,"Right Paren"); break;
		case PLUS    : strcpy(ttype,"Plus sign"); break;
		case MULTIPLY: strcpy(ttype,"Times sign"); break;
		case DIVIDE  : strcpy(ttype,"Divide sign"); break;
		case MINUS   : strcpy(ttype,"Minus sign"); break;
		case EOS : strcpy(ttype,"End of Statement"); break;
		case WHILE : strcpy(ttype,"While"); break;
		case SET: strcpy(ttype,"Set"); break;
		case LCURLY: strcpy(ttype,"Left Curly Brace"); break;
		case RCURLY: strcpy(ttype,"Right Curly Brace"); break;
		case VARIABLE: strcpy(ttype,"Variable"); break;
		case NOTHING: break;
	}

	printf("Token at %3d len=%2d Type=%s",t->location,t->length,ttype);
	if (t->type==INTEGER) printf(" Value=%ld",getValue(t));
	if (t->type==VARIABLE) printf(" Name=%s",t->name);
	printf("\n");
}

void printTree(tree * t,char * prefix) {
	char newPrefix[100];
	printf("%-15s: ",prefix);
	printToken(t->action);
	if (t->action->type==INTEGER) return;
	strcpy(newPrefix,prefix);
	strcat(newPrefix,".L");
	if (t->left==NULL) printf("%s NULL\n",newPrefix);
	else printTree(t->left,newPrefix);
	strcpy(newPrefix,prefix);
	strcat(newPrefix,".R");
	if (t->right==NULL) printf("%s NULL\n",newPrefix);
	else printTree(t->right,newPrefix);
}
