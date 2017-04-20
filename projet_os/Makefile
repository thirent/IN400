all: cesar
	

cesar: cesar.c
	clear
	gcc -o cesar cesar.c -Wall -pthread

run: cesar
	clear
	./cesar test.txt

clear:
	rm -f cesar
