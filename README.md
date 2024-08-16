# CHIP-8   || IN PROGRESS ||
Disassembler and interpreter (emulator?) for the CHIP-8 language, written in C and using the SDL2 library for video, audio, and input. 

The *Disassembler* directory features a program to dump a CHIP-8 binary ROM into assembly language, with descriptions of each command. The *ROMs* directory features binaries to run in the interpreter, and the *Interprer* directory contains the full CHIP 8 implementation. 

Many thanks to Wikipedia for information on CHIP-8, and thanks to @kripod for his database of ROMs. Also thanks to @tobiasvl for his premade fonts (I didn't feel like going through the tedium). 

TODO: 
Test keyboard functionality. 
Test more ROMs, add threading so the delay and sound timers count down at 60hz and the screen is not unnecessarily updated. 
