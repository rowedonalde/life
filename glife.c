#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <unistd.h>
#include <SDL2/SDL.h>

#include "lifelib.h"

#define MICRO_PER_MILLI 1000
#define WAIT_TIME_MS 1000
#define WINDOW_W 500
#define WINDOW_H 500

/*
 * Displays game graphically rather than with ASCII
 */
void gdisplay(int* board, int board_w, int board_h, SDL_Renderer* ren,
    int win_w, int win_h) {
    SDL_Surface* surf;
    surf = SDL_CreateRGBSurface(0, win_w, win_h, 32, 0, 0, 0, 0);
    
    SDL_Rect rect;
    rect.w = win_w / board_w;
    rect.h = win_h / board_h;
    
    int i;
    int j;
    
    for (i = 0; i < board_h; i++) {
        // vertical position for this row:
        rect.y = i * rect.h;
        for (j = 0; j < board_w; j++) {
            // Only try to draw if it's a living cell:
            if (board[i * board_w + j] == ALIVE) {
                // Horizontal position of this cell:
                rect.x = j * rect.w;
                int r = 255;
                int g = 255;
                int b = 255;
                SDL_FillRect(surf, &rect, SDL_MapRGB(surf->format, r, g, b));
            }
        }
    }
    
    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surf);
    SDL_RenderCopy(ren, tex, NULL, NULL);
    SDL_DestroyTexture(tex);

    SDL_RenderPresent(ren);
    SDL_FreeSurface(surf);
    SDL_RenderClear(ren);
}

static int TimerThread(void *wait_ms_ptr) {
    Uint32 lifeTickEvent = SDL_RegisterEvents(1);
    SDL_Event event;
    SDL_memset(&event, 0, sizeof(event));
    event.type = lifeTickEvent;
    event.user.code = 1;
    event.user.data1 = 0;
    event.user.data2 = 0;

    while (1) {
        SDL_Delay(*(int *) wait_ms_ptr);
        SDL_PushEvent(&event);
    }

    return 0;
}

int main(int argc, char** argv) {
    // Process args:
    int wait_ms = WAIT_TIME_MS;
    int win_width = WINDOW_W;
    int win_height = WINDOW_H;
    if (argc == 2) {
        wait_ms = atoi(argv[1]);
    } else if (argc == 3) {
        win_width = atoi(argv[1]);
        win_height = atoi(argv[2]);
    } else if (argc == 4) {
        wait_ms = atoi(argv[1]);
        win_width = atoi(argv[2]);
        win_height = atoi(argv[3]);
    }

    // Put game timer into a thread so we're not constantly spinning
    // and pegging CPU at 100%:
    SDL_CreateThread(TimerThread, "TimerThread", &wait_ms);
    
    // Game setup:
    int width;
    int height;

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
        win_width,
        win_height,
        SDL_WINDOW_SHOWN
    );
    SDL_Renderer* ren = SDL_CreateRenderer(
        main_window,
        -1,
        SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC
    );
    
    // Initial frame:
    gdisplay(even, width, height, ren, win_width, win_height);
    
    // Main Animation Loop:

    // Quit when 0:
    int gamerun = 1;

    // Pause game (but still listen for input) when 0:
    int play_simulation = 1;

    // Play the next generation of life when 1:
    int do_next_tick = 0;

    int tick = 0;
    const Uint8 *key_state = NULL;

    while (gamerun) {
        if (play_simulation && do_next_tick) {
            // The order here is flipped around compared to the
            // ascii version. Here, tick represents which tick
            // we're on, not how many we've finished.
            tick++;
            if (tick % 2) {
                play(even, odd, width, height);
                gdisplay(odd, width, height, ren, win_width, win_height);
            } else {
                play(odd, even, width, height);
                gdisplay(even, width, height, ren, win_width, win_height);
            }
            do_next_tick = 0;
        }

        // SDL doesn't automatically poll for events, including quit:
        SDL_Event event;

        if (SDL_WaitEvent(&event)) {
            // Quit:
            if (event.type == SDL_QUIT) {
                gamerun = 0;
            }

            if (event.type == SDL_USEREVENT) {
                do_next_tick = 1;
            }

            // Pause/play:
            if (event.type == SDL_KEYDOWN) {
                key_state = SDL_GetKeyboardState(NULL);

                if (key_state[SDL_SCANCODE_SPACE]) {
                    // Toggle:
                    play_simulation = (play_simulation - 1) * -1;
                }
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                // Derive game cell coordinates from click:
                int window_x = event.button.x;
                int window_y = event.button.y;

                int cell_width = win_width / width;
                int cell_height = win_height / height;

                int cell_x = window_x / cell_width;
                int cell_y = window_y / cell_height;

                // Toggle cell value:
                int* current;

                if (tick % 2) {
                    current = odd;
                } else {
                    current = even;
                }

                if (current[cell_y * width + cell_x] == ALIVE) {
                    current[cell_y * width + cell_x] = DEAD;
                } else {
                    current[cell_y * width + cell_x] = ALIVE;
                }

                gdisplay(current, width, height, ren, win_width, win_height);
            }
        }
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(main_window);
    SDL_Quit();
    return 0;
}
