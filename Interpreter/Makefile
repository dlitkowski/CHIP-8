chip8 : audiovisual.o audiovisual.h
	cc -g -o chip8 chip8_main.c audiovisual.o `sdl2-config --cflags --libs`

audiovisual.o : audiovisual.c audiovisual.h
		cc -g -c audiovisual.c `sdl2-config --cflags --libs`


clean : chip8
	rm chip8
