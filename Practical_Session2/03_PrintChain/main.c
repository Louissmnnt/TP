#include <stdio.h>
int main(){
	int i = 0;
	printf("Dénbut du print avec la boucle while.\n");
	while (i< 100){
		printf("Valeur : %d \n", i+1);
		i ++;
	}
	printf("Fin du print avec la boucle while.\n");
	printf("Début du print avec la boucle if.\n");
	for (int j = 0; j<100; j++){
		printf("Valeur : %d \n", j+1);
	}
	printf("Fin du print avec la boucle if.\n");
	return 0;

}
