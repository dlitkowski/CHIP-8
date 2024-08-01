#include <stdio.h>

//Attempt to load binary ROM into given memory.
//If this fails return 1. If it succeeds return 0.
int load(char *memory, char *filename){
    

    //Open a file descriptor for the filename provided
    int rom = open(argv[1], O_RDONLY);

    //Abort if the file does not open correctly
    if(rom == -1){
        printf("Failed opening %s, aborting\n", argv[1]);
        return 1;
    }

    //Counter to keep track of memory location
    int address = 0;

    //Increment memory to program location
    memory += 0x200;

    //Iterate through the ROM and read into memory.
    //Opcodes must be converted from big to little endian
    while(1){
        int read = read(rom, memory + address + 1, 1);
        
        if(read == -1){
            break;
        }

        read = read(rom, memory + address, 1);
        address += 2;
    }

    return 0.
}

// Initialize the necessary SDL functionality and return the SDL
int init(){

    SDL_InitSubSystem(SDL_INIT_TIMER || SDL_INIT_AUDIO || SDL_INIT_VIDEO);

}

// Quit
int quit(){

}

void clear_display(){

}

//Convert opcode to assembly instruction
char *convert_opcode(short *opcode){
    
}
