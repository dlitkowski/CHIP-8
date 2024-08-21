# CHIP-8   || IN PROGRESS ||
Disassembler and interpreter (emulator?) for the CHIP-8 language, written in C and using the SDL2 library for video and input, as well as SDL Mixer for audio. The emulater has only been tested on Linux, and will be exclusive to POSIX systems as a result of pthreads use.  

The *Disassembler* directory features a program to dump a CHIP-8 binary ROM into assembly language, with descriptions of each command. The *ROMs* directory features binaries to run in the interpreter, and the *Interprer* directory contains the full CHIP 8 implementation. 

Many thanks to Wikipedia editors for information on CHIP-8, and thanks to @kripod for his database of ROMs. Also thanks to @tobiasvl for his premade fonts (I didn't feel like going through the tedium), and thanks to @Timendus for his battery of test ROMS for CHIP-8. 

TODO: 

Add audio and add threading for the delay/sound timers and display refresh. 
