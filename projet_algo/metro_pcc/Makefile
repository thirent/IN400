all: pcc
	

pcc: vide ./compil/affichage.o ./compil/creation_graphe.o ./compil/dijkstra.o ./compil/recuperation_clic.o ./compil/reperage_chemin.o ./compil/metro_pcc.o
		gcc -Wall -g -o ./compil/metro_pcc ./compil/affichage.o ./compil/creation_graphe.o ./compil/dijkstra.o ./compil/recuperation_clic.o ./compil/reperage_chemin.o ./compil/metro_pcc.o `sdl-config --libs` -lm -lSDL_ttf
	
vide:
	clear
	

./compil/affichage.o: ../affichage/affichage.c
	gcc -c `sdl-config --cflags` -o ./compil/affichage.o ../affichage/affichage.c
	

./compil/creation_graphe.o: ../creation_graphe/creation_graphe.c
	gcc -c -o ./compil/creation_graphe.o ../creation_graphe/creation_graphe.c 
	

./compil/dijkstra.o: ../dijkstra/dijkstra.c
	gcc -c -o ./compil/dijkstra.o ../dijkstra/dijkstra.c
	

./compil/recuperation_clic.o: ../recuperation_clic/recuperation_clic.c
	gcc -c -o ./compil/recuperation_clic.o ../recuperation_clic/recuperation_clic.c
	

./compil/reperage_chemin.o: ../reperage_chemin/reperage_chemin.c
	gcc -c -o ./compil/reperage_chemin.o ../reperage_chemin/reperage_chemin.c
	

./compil/metro_pcc.o: metro_pcc.c
	gcc -c -o ./compil/metro_pcc.o metro_pcc.c
	

run1: all
	clear
	./compil/metro_pcc 1
	

run2: all
	clear
	./compil/metro_pcc 2
	

clean:
	rm -f ./compil/*.o
	rm -f ./compil/metro_pcc
