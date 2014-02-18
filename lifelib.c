#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lifelib.h"

void blank(int* board, int len) {
    int i;
    
    for (i = 0; i < len; i++) {
        board[i] = DEAD;
    }
}

void set_dead(int* board, int x, int y, int width) {
    board[y * width + x] = DEAD;
}

void set_alive(int* board, int x, int y, int width) {
    board[y * width + x] = ALIVE;
}

int count_alive_neighbors(int* board, int x, int y, int width, int height) {
    int count = 0;
    
    // NW:
    if (x && y && board[(y - 1) * width + x - 1] == ALIVE) count++;
    // N:
    if (y && board[(y - 1) * width + x] == ALIVE) count++;
    // NE:
    if (x + 1 < width && y && board[(y - 1) * width + x + 1] == ALIVE) count++;
    // W:
    if (x && board[y * width + x - 1] == ALIVE) count++;
    // E:
    if (x + 1 < width && board[y * width + x + 1] == ALIVE) count++;
    // SW:
    if (x && y + 1 < height && board[(y + 1) * width + x - 1] == ALIVE)
        count++;
    // S:
    if (y + 1 < height && board[(y + 1) * width + x] == ALIVE) count++;
    // SE:
    if (x + 1 < width && y + 1 < height &&
        board[(y + 1) * width + x + 1] == ALIVE) count++;
    
    return count;
}

void display(int* board, int width, int height) {
    int i;
    int j;
    
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            printf("%d", board[i * width + j]);
        }
        printf("\n");
    }
}

int* input_to_board(int* pwidth, int* pheight) {
    int* board = malloc(1);
    char line [MAXWIDTH];
    // Fill the buffer with terminators:
    int i;
    for (i = 0; i < MAXWIDTH; i++) {
        line[i] = '\0';
    }
    int cursor = 0;
    *pheight = 0;
    
    // We assume that all lines are the same length and that all
    // lines are filled until the line break:
    while (scanf("%s", line) > 0) {
        *pwidth = strlen(line);
        *pheight += 1;
        board = (int*) realloc(board, *pwidth * *pheight * sizeof(int));
        int i;
        // Go through this line and put each symbol on the board: 
        for (i = 0; i < *pwidth; i++) {
            if (line[i] == DEADCHAR) {
                board[cursor] = DEAD;
            } else if (line[i] == ALIVECHAR) {
                board[cursor] = ALIVE;
            }
            cursor++;
        }
        // Fill the buffer with terminators again:
        for (i = 0; i < MAXWIDTH; i++) {
            line[i] = '\0';
        }
    }
    
    return board;
}

void play(int* current, int* next, int width, int height) {
    int i;
    int j;
    blank(next, width * height);
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            int living_neighbors =
                count_alive_neighbors(current, j, i, width, height);
            if (current[i * width + j] == ALIVE) {
                // Natural state is dead, since we blank everything.
                // The only way for a cell to survive is if there
                // are exactly 2 or three living neighbors.
                if (living_neighbors == 2 || living_neighbors == 3) {
                    set_alive(next, j, i, width);
                }
            } else {
                // "Any dead cell with exactly three live
                // neighbours becomes a live cell, as if by reproduction."
                if (living_neighbors == 3) {
                    set_alive(next, j, i, width);
                }
            }
        }
    }
}