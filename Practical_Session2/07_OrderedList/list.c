#include <stdio.h>
#include "list.h"

int main(){
int val;
int size;

//Def de la list:
printf("Saisir la taille de la liste : \n");
scanf("%d", &size);
int list[size];
//Implémentation des valeurs de la list:
for (int i = 0 ; i < size ; i ++){
printf("Saisir la %d valeur de la liste : \n", i);
scanf("%d", &list[i]);
}
//Recherche dans la liste de la valeur voulue:
printf("Saisir la valeur à chercher dans la list:\n");
scanf("%d", &val);
findVal(list, size ,val);

return 0;
}
