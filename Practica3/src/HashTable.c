#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <termio.h>
#include <sys/ioctl.h>

#define tmHash 1013

struct hash *hashTable = NULL;

/*definimos los nodos para la tabla*/
struct node {
    int puntero;
    struct node *next;
};

/*se define la Estructura hash*/
struct hash {
    struct node *head;
};

/*Metodo para generar la llave */
long Hash_key(char * palabra){
	char caracter;
	int i;
	int total = 0;
	int mult = 100;
	for(i=0;i<strlen(palabra);i++){
		caracter=palabra[i];
		if (caracter >= 65 && caracter <=90){
		    caracter = caracter + 32;
		}
        else if (caracter >= 128 && caracter <= 165){
		    caracter = 13;
		}
        else{
            caracter=2;
        }
		total += caracter * mult;
		mult = mult/10;
		if (mult ==0){
			mult = 1;
		}
	}
	return total%tmHash;
}

/*Creacion de los nodos*/
struct node * createNode(long punt) {
    struct node *newnode;
    newnode = (struct node *) malloc(sizeof(struct node));
    newnode->puntero = punt;
    newnode->next = NULL;
    return newnode;
}

/*metodo de insercion en la tabla*/
void insertToHash(char * key, long punt) {
    int hashIndex = Hash_key(key);
    struct node *newnode = createNode(punt);
    /*Empezando en la cabeza busca la posicion "hashIndex" */
    if (!hashTable[hashIndex].head) {
        hashTable[hashIndex].head = newnode;
        return;
    }
    /* aAgrega el nuevo nodo*/
    newnode->next = (hashTable[hashIndex].head);
    /*
     * actualiza la cabeza
     */
    hashTable[hashIndex].head = newnode;
    return;
}


void searchInHash(char* key) {
    int hashIndex = Hash_key(key), flag = 0;
    struct node *myNode;
    myNode = hashTable[hashIndex].head;
    if (!myNode) {
        printf("No se encuentra en el sistema\n");
        return;
    }
    int c =0;

do{
    myNode = myNode->next;

    FILE *fichero;
    struct dogType perro;
    fichero = fopen( "datDogs.dat", "r" );

    int numRegistros = ftell(fichero)/sizeof(perro);
    if(c>numRegistros || c<0){break;}

    fseek (fichero, (myNode->puntero-1)*sizeof(perro), SEEK_SET);
    fread (&perro, sizeof(perro), 1, fichero);
    //printf("%ld\n", k);

    if (strncmp(perro.nombre,key,32 ) == 0) {
        printf("NOMBRE PERRO  : %s\n", perro.nombre);
        printf("REGISTRO      : %ld\n", myNode->puntero);
        flag = 1;
    }

    c++;
} while(myNode != NULL);

    printf("FIN DE BUSQUEDA\n" );

    if (!flag)
        printf("No se encuentra en el sistema\n");
    return;
}
void borrarHash(){
    free(hashTable);
    hashTable = NULL;
}
void llenarTabla(){
    printf("Llenando Tabla");
    FILE * ap;
    int r;
    struct dogType perro;
    ap = fopen("datDogs.dat","r");
    long contador=0;
    do{
        fread(&perro,sizeof(perro),1,ap);
        insertToHash(perro.nombre, contador);
        contador++;
    }while (!feof(ap));
    printf ("la tabla se lleno");
    fclose(ap);
    return;
}
