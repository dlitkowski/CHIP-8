#include <SDL.h>
// #include <SDL2/SDL_video.h>
// #include <SDL2/SDL_render.h>

SDL_Renderer* renderer;
SDL_Window* window;

// TODO: Initialize needed SDL features, return -1 on failure.
int init_sdl(){

    // Attempt to initialize needed SDL subsystems, return -1 on failure
    int sdl_init = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

    if (sdl_init != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return -1;
    }

    // Attempt to create a 128x64 a fullscreen SDL window
    window = SDL_CreateWindow("CHIP-8",
                    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                    128, 64, SDL_WINDOW_RESIZABLE);

    if (window == NULL) {
        printf("SDL_WindowCreate Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // Create an SDL Renderer for the created window, abort on failure.
    renderer = SDL_CreateRenderer(window, -1, 0);

    if (renderer == NULL) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Render a startup animation on the SDL window
    for(int i = 0; i < 128; i++){
        for(int j = 0; j < 64; j++){
            SDL_Delay(1);
            SDL_RenderDrawPoint(renderer, i, j);
            SDL_RenderPresent(renderer);
        }
    }

    return 0;
}

// TODO: Clear the display, setting all pixels to 0
int clear_display(){
    printf("CLEAR DISPLAY\n");
    return 0;
}

// TODO: Draw the given sprite of the given height at the given location
int draw_sprite(char* sprite, int height, int x, int y){

    printf("Draw sprite:\n");

    // Print the sprite using 1s and 0s.
    for (int i = 0; i < height; i++){
        printf("Location (%d, %d): %hhb\n", x, y + 1, *(sprite + i));
    }

    return 0;
}

// TODO: Exit SDL
int quit_sdl(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
