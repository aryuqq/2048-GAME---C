joc_2048: joc_2048.c
	gcc -Wall -g -o joc_2048 joc_2048.c -lncurses

run: joc_2048
	./joc_2048