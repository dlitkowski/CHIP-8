#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "audiovisual.h"
#include <time.h>

#include <SDL2/SDL.h>


// Simple function to get the font sprite's location in memory
// for all hex digits. Returns 80 on failure, which will be a
// blank sprite.
int font_get(char number){

        switch(number) {
            case 0: return 0;
            case 1: return 5;
            case 2: return 10;
            case 3: return 15;
            case 4: return 20;
            case 5: return 25;
            case 6: return 30;
            case 7: return 35;
            case 8: return 40;
            case 9: return 45;
            case 0xA: return 50;
            case 0xB: return 55;
            case 0xC: return 60;
            case 0xD: return 65;
            case 0xE: return 70;
            case 0xF: return 75;
        }

        return 80;
}



// Function to load a ROM into the given memory chunk.
// If the file doesn't load return 1.
// If the file is too big for CHIP-8's memory, return 2.
// If it succeeds return 0.
int load(unsigned char* memory, unsigned char* filename){

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

    // Boolean to run the main CPU loop
    // int* is_running = 1;

    // If no arguments are given, print an error and return.
    if (args == 0) {
        printf("ERROR: No arguments given.\n");
        return -1;
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
    unsigned char keys[16];

    // Declare font data, then copy the font data to the start of the memory.
    unsigned char fonts[80] = {0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
                    0x20, 0x60, 0x20, 0x20, 0x70, // 1
                    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
                    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
                    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
                    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
                    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
                    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
                    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
                    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
                    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
                    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
                    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
                    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
                    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
                    0xF0, 0x80, 0xF0, 0x80, 0x80}; // F

    for(int i = 0; i < 80; i++){
        memory[i] = fonts[i];
    }

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

    // Pass the current time as a seed for random functionality
    srand(time(NULL));

    // NOTE: In the opcode assembly notation, the first hex number represents the class of command.
    // If the command includes one or two registers, the sequentially first one is represented as
    // X and the second is represented as Y. The final 1, 2, and 3 hex numbers in the opcode are to be stored as
    // xxxN, xxNN, and xNNN, respectively, and one of these numbers will have bearing on all commands.
    while(1){

        // Update the state of the emulator. Update key presses, and
        // in the case that a quit request is made, exit the program.
        int quit = update_state(keys);

        if(quit){
            printf("Quit request received, aborting.\n");
            SDL_Quit();
            break;
        }

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

                // Set pc to the subroutine's previous address
                // and push the next address after returning to the stack
                sp++;
                stack[sp] = pc;
                pc = op_xNNN - 2;

                break;

            // SKIP OP IF reg_X == xxNN
            case 0x3:

                if(registers[reg_X] == op_xxNN){
                    pc += 2;
                }

                break;

            // SKIP OP IF reg_X != xxNN
            case 0x4:

                if(registers[reg_X] != op_xxNN){
                    pc += 2;
                }

                break;

            // SKIP OP IF reg_X == reg_Y
            case 0x5:

                if(registers[reg_X] == registers[reg_Y]){
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

                    // ADD reg_Y to reg_X, set carry flag in the case of an overflow
                    case 4:

                        if (registers[reg_X] < registers[reg_Y] + registers[reg_X]){
                            registers[0xF] = 1;
                        } else {
                            registers[0xF] = 0;
                        }

                        registers[reg_X] += registers[reg_Y];

                        break;

                    // SUBTRACT reg_Y from reg_X, set carry flag in the case of an underflow
                    case 5:

                        if (registers[reg_X] < registers[reg_Y]){
                            registers[0xF] = 0;
                        } else {
                            registers[0xF] = 1;
                        }

                        registers[reg_X] -= registers[reg_Y];
                        break;

                    // SHIFT reg_X right one bit, store the shifted bit in reg_F
                    case 6:

                        registers[0xF] = 0b1 & registers[reg_X];
                        registers[reg_X] = registers[reg_X] >> 1;
                        break;

                    // SUBTRACT reg_X from reg_Y, store in reg_X
                    case 7:

                        registers[reg_X] = registers[reg_Y] - registers[reg_X];
                        break;

                    // SHIFT reg_X left one bit, store the shifted bit in reg_F
                    case 0xE:

                        registers[0xF] = 0b10000000 & registers[reg_X];
                        registers[reg_X] = registers[reg_X] << 1;
                        break;

                }

                break;

            // SKIP next op if reg_X != reg_Y
            case 0x9:

                if(registers[reg_X] != registers[reg_Y]) {
                    pc += 2;
                }
                break;

            // SET pointer TO xNNN
            case 0xA:

                pointer = op_xNNN;
                break;

            // GOTO reg_0 + op_xNNN
            case 0xB:

                // Set pc to the previous instruction from the desired one
                pc = registers[0] + op_xNNN - 2;
                break;

            // SET reg_X to a random char & xxNN
            case 0xC:

                registers[reg_X] = rand() & op_xxNN;
                break;

            // DRAW pointer SPRITE AT LOCATION reg_X, reg_Y WITH HEIGHT op_xxxN
            case 0xD:

                // Check that the sprite and height do not walk off the allotted memory,
                // if they do print an error message and abort. If not then draw the sprite.
                if (pointer + op_xxxN - 1 > 4096) {
                    printf("Error: sprite of height %d requires nonexistant memory", op_xxNN);
                    return 1;
                }

                int flipped = draw_sprite(memory + pointer, op_xxxN, registers[reg_X], registers[reg_Y]);

                // If the sprite flipped a preset pixel, set reg_F to 1
                if(flipped) {
                    registers[0xF] = 1;
                } else {
                    registers[0xF] = 0;
                }

                break;

            // SKIP the next operation based on if reg_X is pressed or not pressed, depending
            // on the value of xxNN. 9E corresponds to the former and A1 corresponds to the latter
            case 0xE:

                if(op_xxNN == 0x9E){

                    if(keys[registers[reg_X]]){
                        pc += 2;
                        printf("Key is pressed, skip next operation\n");
                    } else {
                        printf("Key is not pressed, skip next operation\n");
                    }

                } else if(op_xxNN == 0xA1){

                    if(keys[registers[reg_X]] == 0){
                        pc += 2;
                        printf("Key is not pressed, skip next operation\n");
                    } else {
                        printf("Key is pressed, do not skip next operation\n");
                    }

                }

                break;

            case 0xF:

                switch(op_xxNN){

                    // ASSIGN reg_X to the delay timer
                    case 0x07:

                        registers[reg_X] = delay_timer;
                        break;

                    // ASSIGN reg_X to the next key pressed
                    case 0x0A:

                        char keypressed = key_wait();
                        registers[reg_X] = keypressed;
                        break;

                    // ASSIGN delay timer to reg_X
                    case 0x15:


                        delay_timer = registers[reg_X];
                        break;

                    // ASSIGN sound timer to reg_X
                    case 0x18:

                        sound_timer = registers[reg_X];
                        break;

                    // ASSIGN pointer += reg_X
                    case 0x1E:

                        pointer += registers[reg_X];
                        break;

                    // ASSIGN pointer to the font location of reg_X's char
                    case 0x29:

                        pointer = font_get(registers[reg_X]);
                        break;

                    // Store the decimal representation of reg_X in the pointer and following locations
                    case 0x33:

                        unsigned int hundreds = registers[reg_X] / 100;
                        unsigned int tens = (registers[reg_X] - hundreds * 100) / 10;
                        unsigned int ones = (registers[reg_X] - hundreds * 100 - tens * 10);
                        memory[pointer] = hundreds;
                        memory[pointer + 1] = tens;
                        memory[pointer + 2] = ones;
                        break;

                    // DUMP registers from reg_0 to reg_X starting at pointer
                    case 0x55:

                        for(int i = 0; i <= reg_X; i++){
                            memory[pointer + i] = registers[i];
                        }

                        break;

                    // LOAD memory starting at pointer into registers reg_0 to reg_X
                    case 0x65:

                        for(int i = 0; i <= reg_X; i++){
                            registers[i] = memory[pointer + i];
                        }

                        break;
                }

                break;
        }

        //Print newline, move to next instruction
        pc+= 2;
        SDL_Delay(1);


        // Decrement delay and sound timers
        // NOTE: these will be moved to a 60hz thread eventually
        if(delay_timer > 0){
            delay_timer--;
        }

        if(sound_timer > 0){
            sound_timer--;
        }

        update_display();

    }

    quit_sdl();
    free(memory);
    return 0;
}
