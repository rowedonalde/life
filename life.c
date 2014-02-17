#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include "lifelib.h"

#define SLEEPTIME 1

int main(int argc, char** argv) {
    int width;
    int height;
    // The board state on even ticks:
    int* even = input_to_board(&width, &height);
    // The board state on odd ticks:
    int* odd = malloc(width * height * sizeof(int));
    
    blank(odd, width * height);

    int tick = 0;
    while (1) {
        if (tick % 2) { // i.e., odd
            display(odd, width, height);
            play(odd, even, width, height);
        } else {
            display(even, width, height);
            play(even, odd, width, height);
        }
        tick++;
        printf("\n");
        sleep(SLEEPTIME);
    }
    
    free(even);
    free(odd);
    return 0;
}