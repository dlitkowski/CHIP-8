#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]){

    //Open a file descriptor for the filename provided
    int binary_rom = open(argv[1], O_RDONLY);

    //Abort if the file does not open correctly
    if(binary_rom == -1){
        printf("Failed opening %s, aborting\n", argv[1]);
        return 1;
    }
    
    //Create an array of shorts for opcodes
    char *opcodes = malloc(sizeof(char) * 290);

    //Read in opcodes
    read(binary_rom, opcodes, 290);

    //Iterate through all opcodes
    for(int i = 0; i < 290; i+= 2){
        
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

            //If the first command is 0
            case 0x0:
                
                if(op_num2 == 0xE0){
                    printf("CLEAR_DISPLAY");
                } else if(op_num2 == 0xEE){
                    printf("RETURN");
                }

                break;

            //If the first command is 1
            case 0x1:

                printf("GOTO %03x", op_num3);
                break;

            //If the first command is 2
            case 0x2:

                printf("CALL %03x", op_num3);
                break;

            //If the first command is 3
            case 0x3:

                printf("SKIP NEXT OP IF V%x == %x", reg1, op_num2);
                break;

            //If the first command is 4
            case 0x4:

                printf("SKIP NEXT OP IF V%x != %x", reg1, op_num2);
                break;

            //If the first command is 5
            case 0x5:

                printf("SKIP NEXT OP IF V%x == V%x", reg1, reg2);
                break;

            //If the first command is 6
            case 0x6:

                printf("ASSIGN V%x = %02x", reg1, op_num2);
                break;

            //If the first command is 7
            case 0x7:

                printf("ASSIGN V%x += %02x", reg1, op_num2);
                break;

            //If the first command is 8
            case 0x8:

                //Read in second command, print assembly instruction based on this
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

            ///If the first command is 9
            case 0x9:

                printf("SKIP NEXT OP IF V%x != V%x", reg1, reg2);
                break;

            //If the first command is 10
            case 10:

                printf("SET I = %02x", op_num3);
                break;

            //If the first command is 11
            case 11:
                printf("GO TO V0 + %03x", op_num3);
                break;

            //If the first command is 12
            case 12:
                printf("SET V%x = RANDOM & %02x", reg1, op_num2);
                break;

            //If the first command is 13
            case 13:
                printf("DRAW I at (V%x, V%x) with height %x", reg1, reg2, op_num1);
                break;

            //If the first command is 14
            case 14:
                if(op_num2 == 0x9E){
                    printf("SKIP NEXT OP IF V%x is pressed", reg1); 
                } else if(op_num2 == 0xA1){
                    printf("SKIP NEXT OP IF V%x is not pressed", reg1); 
                }
                break;

            //If the first command is 15
            case 15:
                
                //Read in second command, print assembly instruction based on this
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
    
    return 0;
}
