#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

int ack(int m, int n);

int main(int argc, char **argv) {
	if (argc<3) {
		printf("Invoke as %s <m> <n>\n",argv[0]);
		return 1;
	}

	int m=atoi(argv[1]);
	int n=atoi(argv[2]);
	int ans=ack(m,n);
	printf("ack(%d,%d)=%d\n",m,n,ans);
	return 0;
}

int ack(int m, int n) {
	if (m==0) return n+1;
	if (m>0 && n==0) return ack(m-1,1);
	return ack(m-1,ack(m,n-1));
 }