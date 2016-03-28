#include <stdlib.h>
#include <stdio.h>
#include <math.h>

double root(double n,double tol);
double howClose(double n, double root);

int main(int argc, char **argv) {

	if (argc != 3) { 
		printf("Invoke %s <n> <e> where <n> is the number to find the square root of, and <e> is the tolerance.\n",argv[0]);
		return 1;
	}
	
	double n=atof(argv[1]);
	double err=atof(argv[2]);
	
	if (n<=0) {
		printf("Invoke %s <n> where <n> is a positive floating point number - you entered %s\n",argv[0],argv[1]);
		return 1;
	}
	
	double rn=root(n,err);
	printf("The square root of %f is %f\n",n,rn);
	return 0;
}

double root(double n,double tol) {

	int iter=0;
	double root=n/2;
	double prev_root = 0;
	double error=howClose(n,root);
	while(error > tol) {
		
		iter++;
		prev_root = root;
		root = (root + n/root)/2;
		if(prev_root == root){
			printf("%s", "error: endless loop: ");
			return root;
			break;
		}
		
		error=howClose(n,root);
		printf("  After %d iterations, root=%f error=%f\n",iter,root,error);
	}
	return root;	
}

double howClose(double n, double root) {

	return fabs(n-root*root); /* fabs is floating point absolute value... fabs(3.6) = 3.6; fabs(-2.5) = 2.5 */
	
}
