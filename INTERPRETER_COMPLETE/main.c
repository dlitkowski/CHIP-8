#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

    // Initialize CPU components
    char memory = malloc(sizeof(char) * 4096);
    char display = malloc(sizeof(char) * 64 * 32);
    char regs = malloc(sizeof(char) * 16);
    int pc = 0x200;
    short i = 0;
    char sp = 0;
    char stack = malloc(48);
    int delay_timer = 0;
    int sound_timer = 0; 

    // Open a file descriptor for the filename provided
    int binary_rom = open(argv[1], O_RDONLY);

    // Abort if the file does not open correctly
    if(binary_rom == -1){
        printf("Failed opening %s, aborting\n", argv[1]);
        return 1;
    }

    // Read in opcodes to memory beginning at address 0x200
    read(binary_rom, memory + 0x200, 0x200);

    // Iterate through all opcodes
    while(1){
        
        // Print
        printf("%x: %hhx%02hhx  ", i/2, memory[pc + 0x200], memory[pc + 0x201]);

        // Extract opcode command
        unsigned char command = memory[pc] >> 4;
        command &= 0x0F;

        // Extract possible opcode registers
        unsigned char reg1 = memory[pc] & 0x0F;
        unsigned char reg2 = memory[pc + 1] >> 4;
        
        // Extract possible opcode specification numbers
        unsigned char op_num1 = memory[pc + 1] & 0x0F;
        unsigned char op_num2 = memory[pc + 1];
        unsigned short op_num3 = (memory[pc + 1]) | (memory[pc] << 8);

        // Print current line
        printf("%x: ", i/2);

        // Decode opcode into command
        switch(command){

            // If the first command is 0
            case 0x0:
                
                // Execute based on NN bits
                switch(op_num2){

                    // If NN = E0 || IN PROGRESS
                    case 0xE0:

                        // Clear display
                        printf("CLEAR_DISPLAY");
                        clear_display(display);
                        update_display(display);
                        break;

                    // If NN = EE || UNSTARTED
                    case 0xEE:

                        // Return from subroutine
                        printf("RETURN");
                        break;

                }

            // If the first command is 1 || IN PROGRESS
            case 0x1:

                printf("GOTO %03x", op_num3);
                pc = op_num3;
                break;

            // If the first command is 2 || UNSTARTED
            case 0x2:

                printf("CALL %03x", op_num3);
                break;

            //If the first command is 3 || UNSTARTED
            case 0x3:

                printf("SKIP NEXT OP IF V%x == %x", reg1, op_num2);
                break;

            //If the first command is 4 || UNSTARTED
            case 0x4:

                printf("SKIP NEXT OP IF V%x != %x", reg1, op_num2);
                break;

            //If the first command is 5 || UNSTARTED
            case 0x5:

                printf("SKIP NEXT OP IF V%x == V%x", reg1, reg2);
                break;

            //If the first command is 6 || IN PROGRESS
            case 0x6:

                //Set Vx to specified number
                printf("ASSIGN V%x = %02x", reg1, op_num2);
                regs[reg1] = op_num2;
                break;

            //If the first command is 7 || IN PROGRESS
            case 0x7:

                //Add specified number to register Vx
                printf("ASSIGN V%x += %02x", reg1, op_num2);
                regs[reg1] += op_num2;
                break;

            //If the first command is 8 || UNSTARTED
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

            //If the first command is 9 || UNSTARTED
            case 0x9:

                printf("SKIP NEXT OP IF V%x != V%x", reg1, reg2);
                break;

            //If the first command is A || IN PROGRESS
            case 0xA:

                //Set index register i to specified digit
                printf("SET I = %02x", op_num3);
                i = op_num3;
                break;

            //If the first command is B || UNSTARTED
            case 0xB:
                printf("GO TO V0 + %03x", op_num3);
                break;

            //If the first command is C || UNSTARTED
            case 0xC:
                printf("SET V%x = RANDOM & %02x", reg1, op_num2);
                break;

            //If the first command is D || UNSTARTED
            case 0xD:

                printf("DRAW I at (V%x, V%x) with height %x", reg1, reg2, op_num1);
                break;

            //If the first command is E || UNSTARTED
            case 0xE:
                if(op_num2 == 0x9E){
                    printf("SKIP NEXT OP IF V%x is pressed", reg1); 
                } else if(op_num2 == 0xA1){
                    printf("SKIP NEXT OP IF V%x is not pressed", reg1); 
                }
                break;

            //If the first command is F || UNSTARTED
            case 0xF:
                
                switch(op_num2){
                    case 0x07:

                        printf("ASSIGM V%x = delay_timer", reg1);
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
        
    }
    pc++

    return 0;
}

void clear_display(char *display){

}

void update_display(char *display){

}
