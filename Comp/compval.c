#include <stdio.h>

int main(){
	int a,b;
	printf("Saisir a et b :\n");
	scanf("%d %d", &a, &b);

	if (a < b){printf("a est inf à b.\n");}
	else if(a > b){printf("a est supp à b.\n");}
	else {printf(" a et b sont égaux\n");}
	
}
