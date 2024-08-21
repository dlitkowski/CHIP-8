#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <pthread.h>

int init_sdl();
int clear_display();
void update_display();
int key_wait();
int update_state(char* keys);
int draw_sprite(char* sprite, char height, char x, char y, pthread_mutex_t* lock);
int quit_sdl();
void beep();
