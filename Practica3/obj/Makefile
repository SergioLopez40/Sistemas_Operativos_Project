all: veterinaria random

veterinaria: p3-dogServer.c p3-dogClient.c #HashTable.o
	gcc -o servidor p3-dogServer.c -pthread
	gcc -o cliente p3-dogClient.c
random: generador1.c
	gcc -o generador generador1.c

clear:
	rm servidor cliente generador
