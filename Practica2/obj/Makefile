all: veterinaria random

veterinaria: p2-dogServer.c p2-dogClient.c #HashTable.o
	gcc -o servidor p2-dogServer.c -pthread
	gcc -o cliente p2-dogClient.c
random: generador1.c
	gcc -o generador generador1.c

clear:
	rm servidor cliente generador
