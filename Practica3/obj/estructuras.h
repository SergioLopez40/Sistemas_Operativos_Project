#ifndef _estructuras_h_
#define  _estructuras_h_

/* Estructura que guarda los datos del cliente*/
struct cliType{
	int clienteFd;
	char *ip;
};
/* Declaracion de la estructura dogType */
struct dogType {
	char nombre[32];
	int edad;
	char raza[16];
	int estatura;
	float peso;
	char sexo;
};

/* Declaracion de la estructura opcionType*/
struct opcionType{
   int opcion;
};

/* Declaracion de la estructura palabra*/
struct wordType{
   char palabra[32];
};

/*struct cliType cli;
struct dogType dg;
struct wordType wd;
struct opcionType oc;

int tamClient =  sizeof(cli);
int tamDog =  sizeof(dg);
int tamWord =  sizeof(wd);
int tamOp = sizeof(oc);
*/
#endif //FIN
