/*
 * lifelib.h
 */

#ifndef __LIFELIB__
#define __LIFELIB__

void blank(int* board, int len);

void set_dead(int* board, int x, int y, int width);

void set_alive(int* board, int x, int y, int width);

int count_alive_neighbors(int* board, int x, int y, int width, int height);

void display(int* board, int width, int height);

int* input_to_board(int* pwidth, int* pheight);

void play(int* current, int* next, int width, int height);

#endif