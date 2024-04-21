#include <stdio.h>
#include <time.h>

int main() {
    struct timespec start_time, end_time;
    long long elapsed_time_ns;
    long int num1 = 123456789123456789;
    long int num2 = 987654321987654321;
    long int result;

    // Obtient le temps de début
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    // Effectue la multiplication
    result = num1 * num2;
    printf("Result of the multiplication of %ld by %ld is : %ld\n", num1, num2, result);

    // Obtient le temps de fin
    clock_gettime(CLOCK_MONOTONIC, &end_time);

    // Calcule le temps écoulé en nanosecondes
    elapsed_time_ns = (end_time.tv_sec - start_time.tv_sec) * 1000000000LL; // Conversion des secondes en nanosecondes
    elapsed_time_ns += end_time.tv_nsec - start_time.tv_nsec; // Ajoute les nanosecondes

    // Affiche le résultat et le temps écoulé
    printf("Temps d'exécution : %lld nanosecondes\n", elapsed_time_ns);

    return 0;
}
