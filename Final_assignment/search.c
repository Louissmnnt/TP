#include <stdio.h>
#include <time.h>

#define SIZE 50

int main() {
    struct timespec start_time, end_time;
    long long elapsed_time_ns;

    // Crée une liste de 50 éléments (nombres de 1 à 50)
    int arr[SIZE];
    for (int i = 0; i < SIZE; i++) {
        arr[i] = i + 1;
    }

    // Élément à rechercher
    int key = 25;

    // Obtient le temps de début
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    // Fonction de recherche binaire
    int low = 0, high = SIZE - 1, mid;
    int found = 0;
    while (low <= high) {
        mid = low + (high - low) / 2;
        if (arr[mid] == key) {
            found = 1;
            break;
        } else if (arr[mid] < key) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    // Obtient le temps de fin
    clock_gettime(CLOCK_MONOTONIC, &end_time);

    // Calcule le temps écoulé en nanosecondes
    elapsed_time_ns = (end_time.tv_sec - start_time.tv_sec) * 1000000000LL; // Conversion des secondes en nanosecondes
    elapsed_time_ns += end_time.tv_nsec - start_time.tv_nsec; // Ajoute les nanosecondes

    // Affiche le résultat de la recherche et le temps écoulé
    if (found) {
        printf("L'élément %d a été trouvé à l'index %d\n", key, mid);
    } else {
        printf("L'élément %d n'a pas été trouvé dans le tableau\n", key);
    }
    printf("Temps d'exécution : %lld nanosecondes\n", elapsed_time_ns);

    return 0;
}
