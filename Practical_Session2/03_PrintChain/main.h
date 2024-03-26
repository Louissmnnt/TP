#include <stdio.h>

void WithWhile(int maxval){
int i = 0;
printf("Dénbut du print avec la boucle while.\n");
while (i < maxval){
  printf("Valeur : %d \n", i+1);
  i ++;
}
printf("Fin du print avec la boucle while.\n");
}

void WithFor(int maxval){
printf("Début du print avec la boucle if.\n");
for (int j = 0; j < maxval ; j++){
   printf("Valeur : %d \n", j+1);
}
printf("Fin du print avec la boucle if.\n");
}
