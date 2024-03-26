#include <stdio.h>
#include <time.h>

void printEachSec() {
    clock_t last_clock = clock(); // Get the initial clock ticks
    int compt = 1;
    while (1) {
        clock_t current_clock = clock(); // Get the current clock ticks

        // Check if a second has elapsed since the last print
        if ((double)(current_clock - last_clock) / CLOCKS_PER_SEC >= 1.0) {
            printf("Seconds elapsed: %d\n", compt);
            last_clock = current_clock; // Update the last clock ticks
	    compt++;
        }
    }
}

int main() {
    printEachSec();
    return 0;
}
