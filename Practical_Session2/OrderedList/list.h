#include <stdio.h>

void findVal(int list[], int size, int value) {
    int count = 0;
    int found = 0;
    int position = -1;

    // Recherche de la valeur dans la liste
    for (int i = 0; i < size; i++) {
        if (list[i] == value) {
            printf("Présente dans la liste en position: %d \n", i+1);
	    count++;
            found = 1;
            if (position == -1) {
                position = i + 1; // Position de l'élément trouvé dans la liste (1-indexed)
            }
        }
    }

    // Affichage des résultats
    if (found) {
        printf("Nombre de fois présent : %d\n", count);
    } else {
        printf("La valeur %d n'est pas présente dans la liste.\n", value);
    }
}
