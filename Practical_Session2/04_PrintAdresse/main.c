#include <stdio.h>

int main() {
    	int variable;
    	printf("Rentrer la valeur :\n");
	scanf("%d", &variable);

  	// Afficher l'adresse de la variable
	printf("La valeur de la variable est : %d\n", variable);
   	printf("L'adresse de la variable est : %p\n", (void *)&variable); //(void *) pour convertir dnas le bon format l'adresse

    return 0;
}
