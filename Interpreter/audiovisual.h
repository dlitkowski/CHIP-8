#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>

int init_sdl();
int clear_display();
void update_display();
int key_wait();
int update_state(char* keys);
int draw_sprite(char* sprite, char height, char x, char y);
int quit_sdl();
