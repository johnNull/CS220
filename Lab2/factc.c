#include<stdio.h>
#include<math.h>
long double factorial(long double n);

int main(int argv,char **argc) {

	long double n,fn;

	for(fn=n=1; fn>0 && fn!=INFINITY; n++) {
		fn=factorial(n);
		printf("factorial(%Lf)=%Lf\n",n,fn);
	}
	return(0);

}

long double factorial(long double n) {
	if(n == 1){return n;}
	else{
		return n * factorial(n-1);
	}
}

