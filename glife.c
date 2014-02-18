#include <stdio.h>
#include <time.h>

#include <unistd.h>
#include <SDL2/SDL.h>

#include "lifelib.h"

#define WINDOW_W 500
#define WINDOW_H 500

/*
 * Displays game graphically rather than with ASCII
 */
void gdisplay(int* board, int width, int height, SDL_Renderer* ren) {
    SDL_Surface* surf;
    surf = SDL_CreateRGBSurface(0, WINDOW_W, WINDOW_H, 32, 0, 0, 0, 0);
    
    SDL_Rect rect;
    rect.w = WINDOW_W / width;
    rect.h = WINDOW_H / height;
    
    int i;
    int j;
    
    for (i = 0; i < height; i++) {
        // vertical position for this row:
        rect.y = i * rect.h;
        for (j = 0; j < width; j++) {
            // Only try to draw if it's a living cell:
            if (board[i * width + j] == ALIVE) {
                // Horizontal position of this cell:
                rect.x = j * rect.w;
                int r = 255;
                int g = 255;
                int b = 255;
                SDL_FillRect(surf, &rect, SDL_MapRGB(surf->format, r, g, b));
                SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surf);
                SDL_RenderCopy(ren, tex, NULL, NULL);
                SDL_DestroyTexture(tex);
            }
        }
    }
    
    SDL_RenderPresent(ren);
    SDL_FreeSurface(surf);
    SDL_RenderClear(ren);
}

int main(int argc, char** argv) {
    // Game setup:
    int width;
    int height;
    // On my machine, this is in microseconds, but that may
    // differ based on implementation, I think:
    clock_t last_frame_time;
    // The board state on even ticks:
    int* even = input_to_board(&width, &height);
    // The board state on odd ticks:
    int* odd = malloc(width * height * sizeof(int));
    blank(odd, width * height);
    
    // SDL Setup:
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* main_window = SDL_CreateWindow(
        "Conway's Game of Life",
        100,
        100,
        500,
        500,
        SDL_WINDOW_SHOWN
    );
    SDL_Renderer* ren = SDL_CreateRenderer(
        main_window,
        -1,
        SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC
    );
    
    // Initial frame:
    gdisplay(even, width, height, ren);
    last_frame_time = clock();
    
    // Main Animation Loop:
    int gamerun = 1;
    int tick = 0;
    while (gamerun) {
        if (clock() - last_frame_time >= 1000000) {
            // The order here is flipped around compared to the
            // ascii version. Here, tick represents which tick
            // we're on, not how many we've finished.
            tick++;
            if (tick % 2) {
                play(even, odd, width, height);
                gdisplay(odd, width, height, ren);
            } else {
                play(odd, even, width, height);
                gdisplay(even, width, height, ren);
            }
            last_frame_time = clock();
        }
        
        // SDL doesn't automatically poll for events, including quit:
        SDL_Event event;
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT) {
            gamerun = 0;
        }
    }
    
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(main_window);
    SDL_Quit();
    return 0;
}