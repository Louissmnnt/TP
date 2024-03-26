#include <stdio.h>
#include "class.h"

int main(){

	int a,b;

	printf("Saisir les valeurs de a et b :\n");
	scanf("%d %d", &a, &b);
	
	printf("La somme vaut : %d \n", sum(a,b));
	return 0;
}
