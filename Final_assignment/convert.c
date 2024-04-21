#include <stdio.h>
#include <time.h>

// Fonction de conversion de Celsius en Fahrenheit
int main() {
    struct timespec start_time, end_time;
    long long elapsed_time_ns;
    double celsius = 15.0;
    double fahrenheit;

    // Obtient le temps de début
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    // Effectue la conversion de Celsius en Fahrenheit
    fahrenheit = (celsius * 9.0 / 5.0) + 32.0;

    // Obtient le temps de fin
    clock_gettime(CLOCK_MONOTONIC, &end_time);

    // Calcule le temps écoulé en nanosecondes
    elapsed_time_ns = (end_time.tv_sec - start_time.tv_sec) * 1000000000LL; // Conversion des secondes en nanosecondes
    elapsed_time_ns += end_time.tv_nsec - start_time.tv_nsec; // Ajoute les nanosecondes

    // Affiche le résultat et le temps écoulé
    printf("%.2f Celsius équivaut à %.2f Fahrenheit\n", celsius, fahrenheit);
    printf("Temps d'exécution : %lld nanosecondes\n", elapsed_time_ns);

    return 0;
}
