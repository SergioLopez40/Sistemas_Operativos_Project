#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
 #include <unistd.h>
#define PORT 3535
#define DATAMAXIMA 5000

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


/*Declaracion estructura txt*/
struct textType{
	char complete[100000+100];
};

/* Metodo para enviar nombre de la mascota a ser buscada*/
void enviarTxt ( int servidorfd, struct textType *text){
  int r = send( servidorfd, text, sizeof(struct textType), 0); //
  if (r == -1){
    perror("Error al enviar el nombre");
    exit(-1);
  }
}

/*Metodo para recibir una cadena de similar a un txt */
void recibirTxt (int clientefd, struct textType *text){
   int total=0, r1=0;
      // Ciclo para recibir la informacion completa enviada por el cliente
   do{
      r1 = recv(clientefd, text, sizeof(struct textType), 0 );
      if(r1==-1){
         perror("Error al recibir del servidor\n");
         exit(-1);
      }
      total = total + r1;
   }while (total < sizeof( struct textType));
}

/*Metodo para enviar una mascota al servidor*/
void enviarMascota ( int servidorfd,struct dogType *dog){
	int r = send( servidorfd, dog, sizeof(struct dogType), 0); //
	if (r == -1){
	    perror("Error al enviar datos");
	    exit(-1);
	}
}

/* Metodo para enviar nombre de la mascota a ser buscada*/
void enviarNombre ( int servidorfd, struct wordType *pal){
  int r = send( servidorfd, pal, sizeof(struct wordType), 0); //
  if (r == -1){
    perror("Error al enviar el nombre");
    exit(-1);
  }
}

/* Metodo para mostar una mascota*/
void mostrarMascota(struct dogType *mascota){
	printf("+--------------------------------|\n")	;
	printf("| Nombre: %s                     \n", mascota->nombre);
	printf("| Edad[años]: %i                       \n", mascota->edad);
	printf("| Raza: %s                       \n", mascota->raza);
	printf("| Estatura[cm]: %i                   \n", mascota->estatura);
	printf("| Peso[Kg]: %f                       \n", mascota->peso);
	printf("| Genero[H/M]: %c                     \n", mascota->sexo);
	printf("+--------------------------------|\n")	;
}

/* Metodo que recibe una mascota desde el servidor*/
void recibirMascota (int servidorfd){
	int total=0, r1=0;
   	struct dogType *mensaje;
   	mensaje = malloc( sizeof( struct dogType ) );
   	// Ciclo para recibir la informacion completa enviada por el cliente
   	do{
      r1 = recv(servidorfd, mensaje, sizeof(struct dogType), 0 );
      if(r1==-1){
         perror("Error al recibir del servidor\n");
         exit(-1);
      }
      total = total + r1;
   }while (total < sizeof( struct dogType));

   int i;
   mostrarMascota(mensaje);
   free(mensaje);
}

/* Metodo para recibir la opcion enviada por el cliente*/
int recibirRespuesta (int servidorfd){
   int total=0, r1=0;
   struct opcionType *mensaje;
   mensaje = malloc( sizeof( struct opcionType ) );
   // Ciclo para recibir la informacion completa enviada por el cliente
   do{
      r1 = recv(servidorfd, mensaje, sizeof(struct opcionType), 0 );
      if(r1==-1){
         perror("Error al recibir del servidor\n");
         exit(-1);
      }
      total = total + r1;
   }while (total < sizeof( struct opcionType));

   int opcionDelCliente = mensaje -> opcion; // Se toma la opcion dada
   free(mensaje);
   return opcionDelCliente;
}

/*Metodo para enviar una mascota al servidor*/
void enviarOpcion(int servidorfd, int opcionAEnviar){
	struct opcionType *opcionCliente;
	opcionCliente = malloc(sizeof(struct opcionType));
	opcionCliente-> opcion = opcionAEnviar;

	int r = send( servidorfd, opcionCliente, sizeof(struct opcionType), 0);
	if (r == -1){
	    perror("Error al enviar datos");
	    exit(-1);
	}
	free(opcionCliente);
}

/* Metodo que verifica que la posicion escrita por el usuario sea valida */
int verificarIndice(int numero, int cantidad){
   int bandera = 0;
   if (numero <= 0 || numero > cantidad){
      printf("Numero de registro no valido\n");
   } else{
      bandera = 1;
   }
   return bandera;
}


/* Metodo que se comunica con el servidor para ver un registro*/
void verRegistro(int servidorfd){
	enviarOpcion( servidorfd, 2 );
	int cantidad = recibirRespuesta(servidorfd); // Recibe la cantidad de registros
   	int numero, bandera= 0;
  	if (cantidad == 0){
      		printf("No hay registros en este momento\n");
   	} else{
      	do{
	    printf("Numero de registros: %i \n", cantidad);
	    printf("Ingrese el numero del registro que desea ver: ");
	    scanf("%i",&numero);
	    bandera = verificarIndice(numero, cantidad); // Se verifica que el indice sea valido
        } while (bandera == 0);
	   enviarOpcion( servidorfd, numero );
	   recibirMascota (servidorfd);
     struct textType *xtext;
     xtext = malloc(sizeof(struct textType));
     printf("VOY A RECIBIR TXT\n" );
     recibirTxt(servidorfd,xtext);
     printf("STO TXT\n" );
     FILE *plano;
     plano = fopen("historiaTemp.txt","a");
     /*posible error*/
     fprintf(plano,"%d", xtext);
     fclose(plano);
     free(xtext);
     /*fin de creacion y escritura del txt*/
     /*ejecuta*/
		 char ejec[25] = "gedit ./";
		 strcat(ejec,"historiaTemp.txt");
		 printf("COMANDO A EJECUTAR: %s\n",ejec);
		  system(ejec);

      int op=0;
      while (op == 1) {
        printf("Ya termino con las modificicaciones de la historia clinica?\n");
        printf("Si = 1, No = 0\n");
        scanf(" %d\n",&op);
      }
      /*se procede a ler este archivo nuevo de texto*/
      char copy[100000];
      char complete[100000+100];
      FILE * archivito;
      archivito = fopen("historiaTemp.txt","r+");
      if(archivito==NULL){
        perror("Perrito, no se abrio el archivo");
        // return -1;
      }
      while (!feof(archivito)) {
      if(fgets(copy,sizeof(copy),archivito) != NULL){
        strcat(complete,copy);
      }

      }
      fclose(archivito);
      /*Vamos a enviar el txt*/
      struct textType *text;
      text = malloc(sizeof(struct textType));
      strcpy(text->complete, complete);
      enviarTxt (servidorfd,text);
      free(text);

    }
}

/*  Metodo que se comunica con el servidor para borrar un registro*/
void borrarRegistro(int servidorfd){
	enviarOpcion( servidorfd, 3 );
	int numero;
	int bandera;
   int cantidad = recibirRespuesta(servidorfd);
   if (cantidad == 0){
      printf("No hay registros en este momento\n");
    } else {
         do{
		      printf("Numero de registros: %i \n", cantidad);
		      printf("Escriba el numero de registro que va a eliminar: ");
		      scanf("%i",&numero);
		      bandera = verificarIndice(numero, cantidad); // Verifica que la posicion sea valida
		   } while (bandera == 0);
		   enviarOpcion( servidorfd, numero);
   }
}

/* Metodo que se comunica con el servidor para buscar un registro, segun su nombre*/
void buscarRegistro(int servidorfd){

   enviarOpcion( servidorfd, 4);
   int cantidad = recibirRespuesta(servidorfd);
   if (cantidad == 0){
        printf("No hay registros en este momento\n");
   } else {
    	char c, nombre[32];
    	while ((c = getchar()) != '\n');
	printf ("Digite el nombre a buscar: ");
	scanf(" %s[^\n]",nombre);

	struct wordType *pal;
	pal = malloc(sizeof(struct wordType));
	strcpy(pal->palabra, nombre);
    	enviarNombre(servidorfd, pal);
    	int op;
    	do{
    	      op = recibirRespuesta(servidorfd);
    	      if (op == 1){
    	         recibirMascota(servidorfd);
    	      }
    	}while(op == 1);
    	if (op == -1){
    		printf("Nombre no encontrado \n");
    	}
    	free(pal);
   }
}

/* Metodo para ingresar cada uno los datos de una mascota y almacenarlos en una estructura */
void ingresarRegistros(int servidorfd){
	struct dogType *datos;
	datos = malloc(sizeof(struct dogType)); // Se separa memoria para la estructura
	char c,aux;
	char nombre[33], raza[17];

	while ((c = getchar()) != '\n');
	printf("Ingrese los datos de la mascota : \n");
	printf("Ingrese el nombre: \n");
	fgets(nombre, 33, stdin);
        nombre[strlen(nombre)-1]='\0';
	strcpy(datos->nombre, nombre);

	//while ((c = getchar()) != '\n');
	printf("Ingrese la edad [en años]: \n");
	scanf(" %i",&datos -> edad);

	while ((c = getchar()) != '\n');
	printf("Ingrese la raza: \n");
	fgets(raza, 17, stdin);
    	raza[strlen(raza)-1]='\0';
	strcpy(datos->raza, raza);

	//while ((c = getchar()) != '\n');
	printf("Ingrese la estatura [en centimetros]: \n");
	scanf(" %i",&datos -> estatura);

	printf("Ingrese la peso [en kilogramos]: \n");
	scanf(" %f",&datos -> peso);

	while ((c = getchar()) != '\n');
	do{
		printf("Ingrese el sexo [H: Hembra, M: Macho]: \n");
		scanf(" %c",&aux);
			datos-> sexo = aux;
		if(!(('h'==aux)||('H'==aux)||('m'==aux)||('M'==aux))){
			printf("Dato Invalido\n");
		}
	}while(! (('h'==aux)||('H'==aux)||('m'==aux)||('M'==aux)));
	enviarOpcion(servidorfd, 1);
	enviarMascota(servidorfd, datos);
	free(datos);		// Se libera la memoria ocupada para la estructura

}

/*Metodo que pide una tecla de confirmacion para continuar.*/
void pausar(){
	char c;
	while ((c = getchar()) != '\n');
	printf("Presione 'Enter' para continuar... ");
	while ( getchar() != '\n');
}
/**
* Main
*/
int main(int argc, char *argv[]){

  	int r,servidorfd;
  	char buf[DATAMAXIMA];  /* en donde es almacenará el texto recibido */
  	struct hostent *x;  /* estructura que recibirá información sobre el nodo remoto */
  	struct sockaddr_in servidor; /* información sobre la dirección del servidor */

 	x=gethostbyname(argv[1]);
  	if (x==NULL){       /* llamada a gethostbyname() */
    	perror("gethostbyname() error\n");
      	exit(-1);
   	}

  	servidorfd = socket(AF_INET,SOCK_STREAM,0);
  	if(servidorfd==-1){
    	perror("Error al crear el socket\n");
      	exit(-1);
   	}

  	servidor.sin_family=AF_INET;
  	servidor.sin_port=htons(PORT);
  	servidor.sin_addr = *((struct in_addr *)x->h_addr);
  	bzero(&(servidor.sin_zero),8);

	  r=connect(servidorfd,(struct sockaddr*)&servidor, sizeof(struct sockaddr));
		if(r==-1){
		perror("Error al conectar al servidor\n");
		exit(-1);
	}

  	int opcion=0;
    	do{
	    printf("+--------------------------+\n");
            printf("|       BIENVENIDO         |\n");
            printf("+--------------------------+\n");
            printf("| 1. Ingresar registro     |\n");
            printf("| 2. Ver  registros        |\n");
            printf("| 3. Borrar registro       |\n");
            printf("| 4. Buscar registro       |\n");
            printf("| 5. Salir                 |\n");
            printf("+--------------------------+\n");
	    printf("Seleccione una opcion:\n");
            scanf("%i",&opcion);
	    switch( opcion ){
		    case 1  :
		        ingresarRegistros(servidorfd);
		        printf("Se ingreso el registro correctamente \n");
				pausar();
				break;
		    case 2  :
		        verRegistro(servidorfd);
				printf("Se ejecuto mostrar el registro correctamente \n");
				pausar();
				break;
		    case 3  :
		        borrarRegistro(servidorfd);
				printf("Se ejecuto borrar el registro correctamente \n");
				pausar();
				break;
		    case 4  :
		        buscarRegistro(servidorfd);
				printf("Se ejecuto buscar el registro correctamente \n");
				pausar();
				break;
		    case 5  :
				break;
		   default :
				break;
	   }
       	system("clear");  // Limpia la pantalla al ejecutar cada opcion

     } while (opcion != 5);

  close(servidorfd); // Se cierra la conexion con el servidor
  printf("Se cerro la conexion al servidor: \n");

}
