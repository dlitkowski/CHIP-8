#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>

// Initialize display window.
// Return 0 on success, 1 on failure
int init_sdl(){

    // Initialize SDL library
    int init_flag = SDL_Init(SDL_INIT_EVERYTHING);
    
    // Return 1 for failure and print a message if init fails
    if(init_flag != 0){
        printf("Error: SDL Library initialization failed\n");
        return 1;    
    }

    // Create new 64x32 centered fullscreen window
    SDL_Window *window = SDL_CreateWindow("CHIP-8 Graphics", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 64, 32,
        SDL_WINDOW_FULLSCREEN);

    // Return 1 for failure and print a message if window creation fails
    if(window == NULL){
        printf("Error: SDL window creation failed\n");
        SDL_Quit();
        return 1;
    }
    
    // Create renderer for our graphics window
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED);
}

int clear_display

int update_display(char *sprite){

}

// Quit
int quit(){

}
