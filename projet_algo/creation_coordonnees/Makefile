all: ./compil/coordonnees.o
	gcc -Wall -g -o coordonnees ./compil/coordonnees.o `sdl-config --libs` -lm -lSDL_ttf

./compil/coordonnees.o: coordonnees.c
	gcc -c `sdl-config --cflags` -o ./compil/coordonnees.o coordonnees.c

run:all
	./coordonnees

clean:
	rm -f ./compil/*.o
	rm -f coordonnees
