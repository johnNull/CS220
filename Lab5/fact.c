#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int fact(char * prefix,int n);

int main(int argc, char **argv) {
	if (argc<2) {
		printf("Invoke as %s <n>\n",argv[0]);
		return 1;
	}

	int n=atoi(argv[1]);
	int ans=fact("",n);
	printf("fact(%d)=%d\n",n,ans);
	return 0;
}

int fact(char *prefix,int n) {
	printf("%s--> fact(%d)\n",prefix,n);
	int ans;
	if (n<3) { ans=n; }
	else {
		char newPref[256]="   ";
		strcat(newPref,prefix);
		ans=n*fact(newPref,n-1);
	}
	printf("%s<--fact(%d)=%d\n",prefix,n,ans);
	return ans;
}
