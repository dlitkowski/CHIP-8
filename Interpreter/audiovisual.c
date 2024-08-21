#include <SDL2/SDL.h>
#include <stdlib.h>
#include <SDL2/SDL_mixer.h>
#include <pthread.h>

// Declare the shared variables to be used in display functions
SDL_Renderer* renderer;
SDL_Window* window;
char display[2048] = {0};
const unsigned char SDL_keynames[16] = {SDL_SCANCODE_X, SDL_SCANCODE_1, SDL_SCANCODE_2,
                    SDL_SCANCODE_3, SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E,
                    SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_Z,
                    SDL_SCANCODE_C, SDL_SCANCODE_4, SDL_SCANCODE_R, SDL_SCANCODE_F,
                    SDL_SCANCODE_V};
Mix_Music* sound;

// Iterate through the display and show
void update_display(pthread_mutex_t* lock){

    pthread_mutex_lock(lock);

    for(int x = 0; x < 64; x++){
        for(int y = 0; y < 34; y++){

            // If the pixel is set, draw white, if not draw black
            if(display[x + y * 64]){

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawPoint(renderer, x, y);

            } else {

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderDrawPoint(renderer, x, y);

            }
        }
    }

    pthread_mutex_unlock(lock);

    SDL_RenderPresent(renderer);
}


// Clear the display, setting all pixels to 0
int clear_display(pthread_mutex_t* lock){

    printf("Clear Display\n");

    pthread_mutex_lock(lock);

    for(int i = 0; i < 2048; i++){
        display[i] = 0;
    }

    pthread_mutex_unlock(lock);

    return 0;
}



// Update the keyboard state, and check if the window has
// recieved a quit request.
int update_state(unsigned char* keys){

    // Send all events to the queue.
    SDL_PumpEvents();

    // If a quit request has been made, return -1, otherwise
    // update keyboard state and return 0.
    SDL_bool quit = SDL_HasEvent(SDL_QUIT);

    if(SDL_TRUE == quit){
        return 1;
    }

    // Get the current state of all keyboard presses, then create an array to
    // parallel keys for checking SDL_keys
    const unsigned char* SDL_keys = SDL_GetKeyboardState(NULL);

    // Iterate through the 16 keys and update all of them to reflect the current state
    for(int i = 0; i < 16; i++){
        keys[i] = SDL_keys[SDL_keynames[i]];
    }

    return 0;
}



// Initialize needed SDL features, return -1 on failure.
int init_sdl(){

    // Attempt to initialize needed SDL subsystems, return -1 on failure
    int init = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_AUDIO);

    if (init != 0) {
        printf("SDL_Init Error: %s %s\n", SDL_GetError(), Mix_GetError());
        return -1;
    }

    if(Mix_Init(MIX_INIT_OGG) != MIX_INIT_OGG) {
        printf("SDL_Mixer ERROR: Failed to initialize OGG support\n");
        return -1;
    } else {
        printf("SDL_Mixer OGG support successful\n");
    }

    // Attempt to create a 1280 x 640 SDL window
    window = SDL_CreateWindow("CHIP-8",
                    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                    1280, 640, SDL_WINDOW_MINIMIZED);

    if (window == NULL) {
        printf("SDL_WindowCreate Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // Create an SDL Renderer for the created window, abort on failure.
    renderer = SDL_CreateRenderer(window, 0, 0);

    if (renderer == NULL) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // Load SDL music
    Mix_OpenAudio(48000, AUDIO_S16SYS, 1, 2048);
    sound = Mix_LoadMUS("./Beep.ogg");

    if(sound == NULL){
        printf("Loading sound file failed: %s\n", Mix_GetError());
    }

    SDL_RenderSetLogicalSize(renderer, 64, 32);

    return 0;
}



// Draw the given sprite of the given height at the given location
int draw_sprite(char* sprite, int height, int x, int y, pthread_mutex_t* lock){

    // Wrap out of bounds locations
    x = x % 64;
    y = y % 32;

    // Record if any pixels are flipped from on to off
    int flipped = 0;

    // printf("Draw sprite of height %d at (%d, %d)\n", height, x, y);

    pthread_mutex_lock(lock);

    // Iterate through the sprite's display locations
    for(int i = 0; i < height; i++){
        for(int j = 0; j < 8; j++){

            // Extract the corresponding pixel on the display.
            char* pixel = display + (y + i) * 64 + x + 7 - j;

            // Check if the sprite bit is set
            if ((*(sprite + i) >> j) & 0b1) {

                // Skip the pixel if it's off the screen
                if(x + 7 -j >= 64 || y + i >= 32) {
                    break;
                }

                // Check if the pixel is currently set or not. If not then
                // draw it, if it is then set flipped to 1.
                if(*pixel) {

                    *pixel = 0;
                    flipped = 1;

                } else {

                    *pixel = 1;

                }
            }
        }
    }

    pthread_mutex_unlock(lock);

    return flipped;
}



// Wait for a keypress, then return the first key pressed
int key_wait(char keys[16]){

    while(1){

        // Iterate through the 16 keys, if one is pushed then return it
        for(int i = 0; i < 16; i++){
            if(keys[i]){
                return i;
            }
        }

        SDL_Delay(16);

    }
}

// beep
void beep(){

    Mix_PlayMusic(sound, 0);

}

// Exit SDL
int quit_sdl(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    Mix_Quit();
    printf("Quitting SDL.\n");
}
