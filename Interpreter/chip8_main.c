#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "audiovisual.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>



// Function to load a ROM into the given memory chunk.
// If the file doesn't load return 1.
// If the file is too big for CHIP-8's memory, return 2.
// If it succeeds return 0.
int load(char* memory, char* filename){

    // Open the given file, return 1 if it fails.
    int rom_fd = open(filename, O_RDONLY);

    if(rom_fd == -1){
        printf("ERROR: Failed opening ROM %s\n", filename);

        return 1;
    }

    // Read the file descripter into memory
    read(rom_fd, memory + 0x200, 0xE00);

    return 0;
}



int main(int args, char* argv[]){

    // If no arguments are given, print an error and return.
    if (args = 0) {
        printf("ERROR: No arguments given.\n");
        return 0;
    }

    // Initialize virtual CPU components and memory.
    // Includes 4096 bytes of memory, 16 registers, a memory pointer,
    // a program counter, a 32 byte stack, a delay and sound timer,
    // and an (array to handle key presses?).
    unsigned char* memory = malloc(4096);
    unsigned char registers[16];
    unsigned short pointer;
    unsigned short pc = 512;
    unsigned short stack[16];
    unsigned char sp = 0;
    unsigned int delay_timer = 0;
    unsigned int sound_timer = 0;
    unsigned char key[16];

    // Attempt to load the given ROM into memory, abort on failure.
    int load_result = load(memory, argv[1]);

    if(load_result != 0){
        printf("Failure loading ROM %s, aborting. Try a compatible ROM.\n", argv[1]);
        return -1;
    }

    // Attempt to initialize SDL, abort on failure.
    int init = init_sdl();

    if(init != 0){
        printf("ERROR: failed to initialize SDL, aborting.\n");
        quit_sdl();
        return -1;
    }

    // NOTE: In the opcode assembly notation, the first hex number represents the class of command.
    // If the command includes one or two registers, the sequentially first one is represented as
    // X and the second is represented as Y. The final 1, 2, and 3 hex numbers in the opcode are to be stored as
    // xxxN, xxNN, and xNNN, respectively, and one of these numbers will have bearing on all commands.
    while(1){

        // Extract opcode command
        unsigned char command = memory[pc] >> 4;
        command &= 0x0F;

        // Extract possible opcode registers
        unsigned short reg_X = memory[pc] & 0x0F;
        unsigned char reg_Y = memory[pc + 1] >> 4 & 0x0F;

        // Extract possible opcode specification numbers
        unsigned char op_xxxN = memory[pc + 1] & 0x0F;
        unsigned char op_xxNN = memory[pc + 1];
        unsigned short op_xNNN = (memory[pc + 1] & 0x00FF) | (reg_X << 8);

        // Decode opcode into command
        switch (command) {

            case 0x0:

                // CLEAR DISPLAY
                if(op_xxNN == 0xE0){
                    clear_display();

                // RETURN FROM SUBROUTINE
                } else if(op_xxNN == 0xEE){

                    // Pop the return address from the stack
                    pc = stack[sp];
                    sp--;

                }

                break;

            // JUMP TO ADDRESS
            case 0x1:

                // Jump to the address before xNNN
                pc = op_xNNN - 2;
                break;

            // CALL SUBROUTINE
            case 0x2:

                // Set pc to the subroutine and push the next address to the stack
                sp++;
                stack[sp] = pc + 2;
                pc = op_xNNN;

                break;

            // SKIP OP IF reg_X == xxNN
            case 0x3:

                if(reg_X == op_xxNN){
                    pc += 2;
                }

                break;

            // SKIP OP IF reg_X != xxNN
            case 0x4:

                if(reg_X != op_xxNN){
                    pc += 2;
                }

                break;

            // SKIP OP IF reg_X == reg_Y
            case 0x5:

                if(reg_X == reg_Y){
                    pc += 2;
                }

                break;

            // SET reg_X TO xxNN
            case 0x6:

                registers[reg_X] = op_xxNN;
                break;

            // ADD xxNN TO reg_X
            case 0x7:

                registers[reg_X] += op_xxNN;
                break;


            case 0x8:

                switch(op_xxxN){

                    // SET reg_X TO reg_Y
                    case 0:

                        registers[reg_X] = registers[reg_Y];
                        break;

                    // SET reg_X |= reg_Y
                    case 1:

                        registers[reg_X] |= registers[reg_Y];
                        break;

                    // SET reg_X &= reg_Y
                    case 2:

                        registers[reg_X] &= registers[reg_Y];
                        break;

                    // SET reg_X ^= reg_Y
                    case 3:

                        registers[reg_X] ^= registers[reg_Y];
                        break;

                    // ADD reg_Y to reg_X
                    case 4:

                        printf("ASSIGN V%x += V%x, possibly set carry flag", reg_X, reg_Y);
                        break;

                    case 5:

                        printf("ASSIGN V%x -= V%x", reg_X, reg_Y);
                        break;

                    case 6:

                        printf("SHIFT V%x right one bit", reg_X);
                        break;

                    case 7:

                        printf("ASSIGN V%x = V%x - V%x", reg_X, reg_Y, reg_X);
                        break;

                    case 0xE:

                        printf("SHIFT V%x left one bit", reg_X);
                        break;

                }

                break;

            case 0x9:

                printf("SKIP NEXT OP IF V%x != V%x", reg_X, reg_Y);
                break;

            // SET pointer TO xNNN
            case 0xA:

                pointer = op_xNNN;
                break;

            // GOTO reg_0 + op_xNNN
            case 0xB:

                // Set pc to the previous instruction from the desired one
                pc = registers[0] + op_xNNN;
                break;

            case 0xC:
                printf("SET V%x = RANDOM & %02x", reg_X, op_xxNN);
                break;

            // DRAW pointer SPRITE AT LOCATION reg_X, reg_Y WITH HEIGHT op_xxxN
            case 0xD:

                // Check that the sprite and height do not walk off the allotted memory,
                // if they do print an error message and abort. If not then draw the sprite.
                if (pointer + op_xxxN - 1 > 4096) {
                    printf("Error: sprite of height %d requires nonexistant memory", op_xxNN);
                    return 1;
                }

                draw_sprite(memory + pointer, op_xxxN, registers[reg_X], registers[reg_Y]);
                break;

            case 0xE:

                if(op_xxNN == 0x9E){
                    printf("SKIP NEXT OP IF V%x is pressed", reg_X);
                } else if(op_xxNN == 0xA1){
                    printf("SKIP NEXT OP IF V%x is not pressed", reg_X);
                }
                break;

            case 0xF:

                switch(op_xxNN){

                    case 0x07:

                        printf("ASSIGn V%x = delay_timer", reg_X);
                        break;

                    case 0x0A:

                        printf("ASSIGN V%x = get_key", reg_X);
                        break;

                    case 0x15:

                        printf("ASSIGN delay_timer = V%x", reg_X);
                        break;

                    case 0x18:

                        printf("ASSIGN sound_timer = V%x", reg_X);
                        break;

                    case 0x1E:

                        printf("ASSIGN pointer += V%x", reg_X);
                        break;

                    case 0x29:

                        printf("ASSIGN pointer = sprite_address(V%x)", reg_X);
                        break;

                    case 0x33:

                        printf("CONVERT V%x to decimal, store in pointer", reg_X);
                        break;

                    case 0x55:

                        printf("DUMP registers from V0 to V%x starting at pointer", reg_X);
                        break;

                    case 0x65:

                        printf("LOAD memory starting at pointer into registers V0 to V%x", reg_X);
                        break;
                }

                break;
        }

        //Print newline, move to next instruction
        pc+= 2;
        SDL_Delay(16);

    }

    quit_sdl();
    return 0;
}
