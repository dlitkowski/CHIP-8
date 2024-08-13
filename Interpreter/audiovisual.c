#include <SDL.h>
// #include <SDL2/SDL_video.h>
// #include <SDL2/SDL_render.h>

SDL_Renderer* renderer;
SDL_Window* window;

// TODO: Initialize needed SDL features, return -1 on failure.
int init_sdl(){

    // Attempt to initialize needed SDL subsystems, return -1 on failure
    int init = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

    if (init != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return -1;
    }

    // Attempt to create a 128x64 a fullscreen SDL window
    window = SDL_CreateWindow("CHIP-8",
                    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                    1280, 640, 0);

    if (window == NULL) {
        printf("SDL_WindowCreate Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // Create an SDL Renderer for the created window, abort on failure.
    renderer = SDL_CreateRenderer(window, 1, 0);

    if (renderer == NULL) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_RenderSetLogicalSize(renderer, 64, 32);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Render a startup animation on the SDL window
    for(int i = 0; i < 32; i++){
        SDL_Delay(32);
        SDL_RenderDrawLine(renderer, 0, i, 64, i);
        SDL_RenderPresent(renderer);
    }

    SDL_Delay(1000);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    return 0;
}

// Clear the display, setting all pixels to 0
int clear_display(){

    printf("Clear Display\n");

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    return 0;
}

// TODO: Draw the given sprite of the given height at the given location
int draw_sprite(char* sprite, int height, int x, int y){

    printf("Draw sprite of height %d at (%d, %d)\n", height, x, y);

    // Set the SDL renderer color to white, then draw the sprite to
    // the renderer using bit shifting.

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for(int i = 0; i < height; i++){
        for(int j = 0; j < 8; j++){
            if ((*(sprite + i) >> j) & 0b1) {
                SDL_RenderDrawPoint(renderer, x + 8 - j, y + i);
            }
        }
    }

    SDL_RenderPresent(renderer);

    return 0;
}

// TODO: Exit SDL
int quit_sdl(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
