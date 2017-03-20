all: coordonnees.o
	gcc -Wall -g -o coordonnees coordonnees.o `sdl-config --libs` -lm -lSDL_ttf

coordonnees.o: coordonnees.c
	gcc -c `sdl-config --cflags` coordonnees.c

run:all
	./coordonnees
