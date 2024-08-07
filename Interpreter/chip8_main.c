#include <stdio.h>
#include <audiovisual.h>


// Function to load a ROM into the given memory chunk.
// If this fails return 1. If it succeeds return 0.
int load(char *memory, char *filename){

    // Open the given file, return 1 if it fails.
    int rom_fd = open(filename, O_RDONLY);
    if(rom_fd == -1){
        printf("ERROR: Failed opening ROM %s", filename);
        return 1;
    }

    // Take the length of the program, if it's too large return 2
    // takelength(rom)
    // if (length == toobig){
    // return 2
    //}


    int address = 0;
    memory += 0x200;


    // Opcodes must be converted from big to little endian
    while(1){
        int read = read(rom, memory + address + 1, 1);

        if(read == -1){
            break;
        }

        read = read(rom, memory + address, 1);
        address += 2;
    }

    return 0
}



int main(int args, char *argv[]){

    // Initialize virtual CPU components and memory
    unsigned char memory = malloc(4096);
    unsigned char registers[16];
    unsigned short i_pointer;
    unsigned short pc = 512;
    unsigned short stack[16];
    unsigned char sp = 0;
    unsigned int delay_timer = 0;
    unsigned int sound_timer = 0;
    unsigned char key[16];

    // Attempt to load the given ROM into memory, abort on failure.
    int load_result = load(memory, argv[1])

    if(load_result != 0){
        printf("Failure loading ROM %s, aborting. Try a compatible ROM.", argv[1]);
    }

    for(int i = 0; i < 3584; i++){
        printf("%s : %hx", i, memory[i + 512]);
    }

    // Attempt to initialize SDL, abort on failure.
    int init = init_sdl();

    if(load_result )


    // NOTE: In the opcode assembly notation, the first hex number represents the class of command.
    // If the command is to include one or two registers, the sequentially first one is represented as
    // X and the second is represented as Y. The final 1, 2, and 3 hex numbers in the opcode are to be stored as
    // xxxN, xxNN, and xNNN, respectively, and one of these numbers will have bearing on all commands.
    while(1){

        //Print
        printf("%x: %02hhx%02hhx  ", i/2, opcodes[i], opcodes[i+1]);

        //Extract opcode command
        unsigned char command = opcodes[i] >> 4;
        command &= 0x0F;

        //Extract possible opcode registers
        unsigned short reg_X = opcodes[i] & 0x0F;
        unsigned char reg_Y = opcodes[i + 1] >> 4 & 0x0F;

        //Extract possible opcode specification numbers
        unsigned char op_xxxN = opcodes[i + 1] & 0x0F;
        unsigned char op_xxNN = opcodes[i + 1];
        unsigned short op_xNNN = (opcodes[i + 1] & 0x00FF) | (reg1 << 8);

        //Print current line
        printf("%x: ", i/2);

        //Decode opcode into command
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

                // Jump to op_num3's address
                pc = op_xNNN;
                break;

            // CALL SUBROUTINE
            case 0x2:

                // Set pc to the subroutine and push the next address to the stack
                sp++;
                stack[sp] = pc + 2;
                pc = op_num3;

                break;

            // SKIP OP IF reg_X == xxNN
            case 0x3:

                if(reg_X == xxNN){
                    pc += 4;
                } else {
                    pc += 2;
                }

                break;

            // SKIP OP IF reg_X != xxNN
            case 0x4:

                if(reg_X != xxNN){
                    pc += 4;
                } else {
                    pc += 2;
                }

                break;

            // SKIP OP IF reg_X == reg_Y
            case 0x5:

                if(reg_X != reg_Y){
                    pc += 4;
                } else {
                    pc += 2;
                }

                break;

            // SET reg_X to xxNN
            case 0x6:

                registers[reg_X] = op_xxNN;
                break;

            // ADD xxNN to reg_X
            case 0x7:

                registers[reg_X] += xxNN;
                break;


            case 0x8:

                switch(op_xxxN){

                    // ASSIGN reg_X to reg_Y
                    case 0:

                        printf("ASSIGN V%x = V%x", reg_X, reg_Y);
                        break;


                    case 1:

                        printf("ASSIGN V%x |= V%x", reg1, reg2);
                        break;

                    case 2:

                        printf("ASSIGN V%x &= V%x", reg1, reg2);
                        break;

                    case 3:

                        printf("ASSIGN V%x ^= V%x", reg1, reg2);
                        break;

                    case 4:

                        printf("ASSIGN V%x += V%x, possibly set carry flag", reg1, reg2);
                        break;

                    case 5:

                        printf("ASSIGN V%x -= V%x", reg1, reg2);
                        break;

                    case 6:

                        printf("SHIFT V%x right one bit", reg1);
                        break;

                    case 7:

                        printf("ASSIGN V%x = V%x - V%x", reg1, reg2, reg1);
                        break;

                    case 0xE:

                        printf("SHIFT V%x left one bit", reg1);
                        break;

                }

                break;

            case 0x9:

                printf("SKIP NEXT OP IF V%x != V%x", reg1, reg2);
                break;

            case 0xA:

                printf("SET I = %02x", op_num3);
                break;

            case 0xB:
                printf("GO TO V0 + %03x", op_num3);
                break;

            case 0xC:
                printf("SET V%x = RANDOM & %02x", reg1, op_num2);
                break;

            // DRAW I SPRITE AT LOCATION reg_X, reg_Y WITH HEIGHT op_xxNN
            case 0xD:

                // Check that the sprite and height do not walk off the allotted memory,
                // if they do print an error message and abort.
                if (i_pointer + height - 1 > 4096) {
                    printf("Error: sprite of height %d requires nonexistant memory", op_xxNN);
                    return
                }


                char[] sprite[op_xxNN] =



                break;

            case 0xE:
                if(op_num2 == 0x9E){
                    printf("SKIP NEXT OP IF V%x is pressed", reg1);
                } else if(op_num2 == 0xA1){
                    printf("SKIP NEXT OP IF V%x is not pressed", reg1);
                }
                break;

            case 0xF:

                switch(op_num2){

                    case 0x07:

                        printf("ASSIGn V%x = delay_timer", reg1);
                        break;

                    case 0x0A:

                        printf("ASSIGN V%x = get_key", reg1);
                        break;

                    case 0x15:

                        printf("ASSIGN delay_timer = V%x", reg1);
                        break;

                    case 0x18:

                        printf("ASSIGN sound_timer = V%x", reg1);
                        break;

                    case 0x1E:

                        printf("ASSIGN I += V%x", reg1);
                        break;

                    case 0x29:

                        printf("ASSIGN I = sprite_address(V%x)", reg1);
                        break;

                    case 0x33:

                        printf("CONVERT V%x to decimal, store in I", reg1);
                        break;

                    case 0x55:

                        printf("DUMP registers from V0 to V%x starting at I", reg1);
                        break;

                    case 0x65:

                        printf("LOAD memory starting at I into registers V0 to V%x", reg1);
                        break;
                }

                break;
        }

        //Print newline, move to next instruction
        printf("\n");

    }

    quit_sdl();
}
