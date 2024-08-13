chip8 : audiovisual.o audiovisual.h
	cc -o chip8 chip8_main.c audiovisual.o `sdl2-config --cflags --libs`

audiovisual.o : audiovisual.c audiovisual.h
		cc -c audiovisual.c `sdl2-config --cflags --libs`


clean : chip8
	rm chip8
