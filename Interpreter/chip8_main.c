#include <stdio.h>



// Function to load a ROM into the given memory chunk.
// If this fails return 1. If it succeeds return 0.
int load(char *memory, char *filename){

    // Open the given file, return -1 if it fails.

    int rom = open(argv[1], O_RDONLY);


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

    return 0
}



int main(int args, char *argv[]){

    // Initialize virtual CPU components and memory
    unsigned char memory = malloc(4096);
    unsigned char registers[16];
    unsigned short i;
    unsigned short pc = 512;
    unsigned short stack[16];
    unsigned char sp = 0;
    unsigned int delay_timer = 0;
    unsigned int sound_timer = 0;
    unsigned char key[16];
    long display[32];

    // Attempt to load the given ROM into memory
    int load_result = load(memory, argv[1]);

    if(load_result == 1){
        printf("ROM %s failed loading, aborting", argv[1]);
        return;
    }

    for(int i = 0; i < 3584; i++){
        printf("%s : %hx", i, memory[i + 512]);
    }

    while(1){

        //Print
        printf("%x: %02hhx%02hhx  ", i/2, opcodes[i], opcodes[i+1]);

        //Extract opcode command
        unsigned char command = opcodes[i] >> 4;
        command &= 0x0F;

        //Extract possible opcode registers
        unsigned short reg1 = opcodes[i] & 0x0F;
        unsigned char reg2 = opcodes[i + 1] >> 4 & 0x0F;

        //Extract possible opcode specification numbers
        unsigned char op_num1 = opcodes[i + 1] & 0x0F;
        unsigned char op_num2 = opcodes[i + 1];
        unsigned short op_num3 = (opcodes[i + 1] & 0x00FF) | (reg1 << 8);

        //Print current line
        printf("%x: ", i/2);

        //Decode opcode into command
        switch(command){

            case 0x0:

                if(op_num2 == 0xE0){

                    // Call function to clear display
                    clear_display();

                } else if(op_num2 == 0xEE){

                    // Pop the return address from the stack
                    pc = stack[sp];
                    sp--;

                }

                break;

            case 0x1:

                // Jump to op_num3's address
                pc = op_num3;
                break;

            case 0x2:

                // Set pc to the subroutine and push the next address to the stack
                sp++;
                stack[sp] = pc + 2;
                pc = op_num3;

                break;

            case 0x3:

                printf("SKIP NEXT OP IF V%x == %x", reg1, op_num2);
                break;

            case 0x4:

                printf("SKIP NEXT OP IF V%x != %x", reg1, op_num2);
                break;

            case 0x5:

                printf("SKIP NEXT OP IF V%x == V%x", reg1, reg2);
                break;

            case 0x6:

                printf("ASSIGN V%x = %02x", reg1, op_num2);
                break;

            case 0x7:

                printf("ASSIGN V%x += %02x", reg1, op_num2);
                break;

            case 0x8:

                switch(op_num1){

                    case 0:

                        printf("ASSIGN V%x = V%x", reg1, reg2);
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

            case 10:

                printf("SET I = %02x", op_num3);
                break;

            case 11:
                printf("GO TO V0 + %03x", op_num3);
                break;

            case 12:
                printf("SET V%x = RANDOM & %02x", reg1, op_num2);
                break;

            case 13:
                printf("DRAW I at (V%x, V%x) with height %x", reg1, reg2, op_num1);
                break;

            case 14:
                if(op_num2 == 0x9E){
                    printf("SKIP NEXT OP IF V%x is pressed", reg1);
                } else if(op_num2 == 0xA1){
                    printf("SKIP NEXT OP IF V%x is not pressed", reg1);
                }
                break;

            case 15:

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

    unsigned short opcode;
}
