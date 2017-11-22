#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <signal.h> //Para usar la funcion signal
#include <ctype.h> //Para convertir a minuscula
#include <pthread.h>

#define PORT 3535
#define BACKLOG 32 //numero maximo de conexiones
int serverfd;
int clientesActuales = 0;

/* Estructura que guarda los datos de un cliente*/
struct cliType{
	int clienteFd;
	char *ip;
};

/* Funcion que se ejecutara cuando se cierre el servidor con ctrl + c*/
void salida(){
	printf ("Ha salido del servidor\n");
	close(serverfd);
	exit(0);
}

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

/*Metodo para enviar un entero al cliente*/
void enviarOpcion(int clientefd, int opcionAEnviar){
	struct opcionType *opcionCliente;
	opcionCliente = malloc(sizeof(struct opcionType));
	opcionCliente-> opcion = opcionAEnviar;

	int r = send( clientefd, opcionCliente, sizeof(struct opcionType), 0);
	if (r == -1){
	    perror("Error al enviar datos");
	    exit(-1);
	}
	free(opcionCliente);
}

/*Metodo para enviar una mascota al cliente*/
void enviarMascota( int clientefd,struct dogType *dog){
	int r = send( clientefd, dog, sizeof(struct dogType), 0); //
	if (r == -1){
	    perror("Error al enviar datos");
	    exit(-1);
	}
}

/*Metodo para recibir una cadena de caracteres enviada por un cliente */
void recibirPalabra (int clientefd, struct wordType *pal){
   int total=0, r1=0;
      // Ciclo para recibir la informacion completa enviada por el cliente
   do{
      r1 = recv(clientefd, pal, sizeof(struct wordType), 0 );
      if(r1==-1){
         perror("Error al recibir del servidor\n");
         exit(-1);
      }
      total = total + r1;
   }while (total < sizeof( struct wordType));
}

/*Metodo para recibir una mascota enviada por un cliente*/
void recibirMascota (int clientefd, struct dogType *dog){
   int total=0, r1=0;
   do{
      r1 = recv(clientefd, dog, sizeof(struct dogType), 0 );
      if(r1==-1){
         perror("Error al recibir del servidor\n");
         exit(-1);
      }
      total = total + r1;
   }while (total < sizeof( struct dogType));
}

/* Metodo para recibir la opcion enviada por el cliente*/
int recibirOpcion (int clientefd){
   int total=0, r1=0;
   struct opcionType *mensaje;
   mensaje = malloc( sizeof( struct opcionType ) );
   // Ciclo para recibir la informacion completa enviada por el cliente
   do{
      r1 = recv(clientefd, mensaje, sizeof(struct opcionType), 0 );
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

/*Metodo para guardar datos del usuario que iran en el archivo archivo log*/
void guardarLog( void *log, int tama ){
        FILE *ap;
        int r;
        ap = fopen( "serverDogs.log", "a+");
	if (ap == NULL){
	    perror("Error al abrir el archivo");
	    exit(-1);
	}

  	r = fwrite( log,tama ,1, ap);
	if (r != 1){
	  perror("No se pudo escribir");
	  exit(-1);
	}
	r = fclose(ap);
	if ( r != 0){
	  perror("No se pudo cerrar");
	  exit(-1);
	}
}

/* Metodo actualiazar los logs de usuario */
void registrar(int opcion, int numeroDeMascotaSeleccionada, char *nombreBuscado, char *ip){
	time_t t;
  	struct tm *tm;
  	char fechayhora[100];
	char opcionElegida[13];
  	t = time( NULL );
  	tm = localtime( &t );

	char cadena[33] = "[Fecha ";
	char cua[2];
  	int tam;

	strftime( fechayhora, 100, "%Y%m%dT%H%M%S]", tm);

  	strncat(cadena,fechayhora, 16);
  	strncat(cadena," Cliente [",10);
   	guardarLog(cadena, sizeof(cadena));

	tam = strlen(ip);
	guardarLog(ip,tam);

   switch( opcion ){
      case 1 : strcpy( opcionElegida, "][insercion][");
         break;
      case 2 : strcpy( opcionElegida, "][ lectura ][");
         break;
      case 3 : strcpy( opcionElegida, "][ borrado ][");
         break;
      case 4 : strcpy( opcionElegida, "][busqueda ][");
         break;
      default: ;
   }
   guardarLog(opcionElegida,13);

	if(opcion == 4 || opcion == 1){
      		tam = strlen(nombreBuscado);
		guardarLog(nombreBuscado, tam );
	}else{
		if(numeroDeMascotaSeleccionada < 10){
			char str[1];
			sprintf(str, "%d",numeroDeMascotaSeleccionada );
			guardarLog(str, sizeof(str));
		}
		else {
			char str[2];
			sprintf(str, "%d",numeroDeMascotaSeleccionada );
			guardarLog(str, sizeof(str));
		}

	}
	strcpy(cua,"]\n");
	guardarLog(cua,2);
}



/* Metodo que guarda una mascota en el archivo dataDogs.dat*/
void guardar(struct dogType *mascota){
   size_t r;
   FILE *ap;
   ap = fopen("dataDogs.dat", "a+");
   if (ap == NULL){
      perror("Error al abrir el archivo");
      exit(-1);
   }
   r = fwrite(mascota, sizeof(struct dogType), 1, ap); // Se guarda una estructura en el archivo
   if (r != 1){
      perror("No se pudo escribir");
      exit(-1);
   }
   r = fclose(ap);
   if ( r != 0){
      perror("No se pudo cerrar");
      exit(-1);
   }
}

/* Metodo para cargar un registro dependiendo de su posicion en el archivo*/
void cargar(int posicion, int clientefd){
   size_t r;
   FILE *ap;
   ap = fopen("dataDogs.dat", "r");
   if (ap == NULL){
      perror("Error al abrir el archivo");
      exit(-1);
   }
   int i=0;
   struct dogType *mascota;
   mascota = malloc(sizeof(struct dogType)); // Se separa memoria para la estructura
   int f1;
   f1 = fseek(ap, (posicion-1)*sizeof(struct dogType), SEEK_SET); // ubica el indicador de posicion del fichero en la posicion buscada
   if (f1 != 0){
      perror("No se pudo indicar la posicion");
      exit(-1);
   }
   r = fread(mascota, sizeof(struct dogType),1, ap); // Se lee la mascota que se busca
   if (r != 1){
      perror("No se pudo leer");
      exit(-1);
   }
   r = fclose(ap);  // Se cierra el flujo de archivos
   if ( r != 0){
      perror("No se pudo cerrar");
      exit(-1);
   }

		 	 enviarMascota(clientefd, mascota);
   		    free(mascota);      // Se libera la memoria ocupada por la estructura

	 /*Aqui comienza lo del txt*/
	 char historia[15];
	 sprintf(historia,"%ld.txt",posicion);
	 /*verifico si existe el archivo de texto plano*/
		 FILE* plano=fopen(historia,"r");
		 printf("Voy a escribir\n" );
	 if(plano == NULL){
printf("Voy a escribirK\n" );
		 /*Manejo el archivo .dat en caso que sea necesario*/
		 FILE *pFichero;
		 /* abro el archivo .dat para lectura*/
		 pFichero = fopen("dataDogs.dat","r");

		 printf("Voy a escribir\n" );
		 fseek(pFichero, (posicion-1)*sizeof(struct dogType), SEEK_SET);
		 /*leo las variables */
		 struct dogType perrox;
		 fread(&perrox,sizeof(struct dogType),1,pFichero);
		 /*variables*/
		 printf("Voy a escriir\n" );
		 plano = fopen(historia,"a");
		 printf("Voy a escriir\n" );
		 fprintf(plano, "----------------HISTORIA CLINICA-------------------- \n",posicion);
		 fprintf(plano, "NOMBRE:   %s\n", perrox.nombre);
		 fprintf(plano, "EDAD:     %d\n", perrox.edad);
		 fprintf(plano, "RAZA:     %s\n", perrox.raza);
		 fprintf(plano, "ESTATURA: %d\n", perrox.estatura);
		 fprintf(plano, "PESO:     %f\n", perrox.peso);
		 fprintf(plano, "SEXO:     %c\n", perrox.sexo);
		 fprintf(plano, "No.HISTORIA: %d\n", posicion);
		 fprintf(plano, "---------------COMENTARIOS VETERINARIA--------------- \n",posicion);
		 fclose(plano);
		 printf("Voy a escriir\n" );
		 /*se procede a ler este archivo nuevo de texto*/

		 char copy[100000];
	   char complete[100000+100];
	   FILE * archivito;
	   archivito = fopen(historia,"r+");
	   if(archivito==NULL){
	     perror("Perrito, no se abrio el archivo");
	    //  return 0;
	   }
	 while (!feof(archivito)) {
	   if(fgets(copy,sizeof(copy),archivito) != NULL){
	     strcat(complete,copy);
	   }

	 }

	 printf("%s\n",complete );

	 fclose(archivito);
	 /*Vamos a enviar el txt*/
		 struct textType *text;
 		 text = malloc(sizeof(struct textType));
 		 strcpy(text->complete, complete);
		 printf("SE ENVO\n" );
		 enviarTxt (clientefd,text);
		 free(text);
		 /*se recibe lo que envia el usuario*/
		 struct textType * xtext;
		 xtext = malloc(sizeof(struct textType));
		 recibirTxt(clientefd, xtext);
		 FILE *plano;
		 plano = fopen(historia,"a");
		 /*posible error*/
		 fprintf(plano,"%d", xtext);
		 fclose(plano);
		 free(xtext);

	 }

	 else{printf("Voy a escribirFUERA\n" );
		 /*se procede a ler este archivo nuevo de texto*/
		 char copy[100000];
	   char complete[100000+100];
	   FILE * archivito;
	   archivito = fopen(historia,"r+");
	   if(archivito==NULL){
	     perror("Perrito, no se abrio el archivo");
	    //  return -1;
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
		 enviarTxt (clientefd,text);
		 free(text);
		 /*se recibe lo que envia el usuario*/
		 struct textType *xtext;
		 xtext = malloc(sizeof(struct textType));
		 recibirTxt(clientefd,xtext);
		 FILE *plano;
		 plano = fopen(historia,"a");
		 /*posible error*/
		 fprintf(plano,"%d", xtext);
		 fclose(plano);
		 free(xtext);
		}



}

/* Metodo que borra un registro del archivo*/
void borrarRegistroDelArchivo(int numero, int cantidad){
   size_t r;
   FILE *ap;
   int bandera,i=0, k=0;;
   ap = fopen("dataDogs.dat", "a+b");
   if (ap == NULL){
      perror("Error al abrir el archivo");
      exit(-1);
   }

   struct dogType lista [cantidad-1]; // Arreglo en donde se guardaran las estructuras temporalmente
   struct dogType *mascota;
   mascota = malloc(sizeof(struct dogType));  // Se separa memoria para la estructura

   for(i; i < cantidad; i++){   // Copia cada registro del archivo en el arreglo (Excepto el que va a ser eliminado)
      r = fread(mascota, sizeof(struct dogType), 1, ap);
      if ( i != numero-1){
         lista[k] = *mascota;
         k = k+1;
      }
      if (r != 1){
         perror("No se pudo leer");
         exit(-1);
      }
   }
   r = fclose(ap);
   if ( r != 0){
      perror("No se pudo cerrar");
      exit(-1);
   }

   ap = fopen("dataDogs.dat", "w+"); // Se abre el archivo en modo w+ para que sobreescriba lo que hay en el
   if (ap == NULL){
      perror("Error al abrir el archivo");
      exit(-1);
   }
   int j=0;
   for (j; j<cantidad-1 ; j++){  // Ciclo para guardar cada estructura en el archivo, sobreescribiendo lo anterior
      *mascota = lista[j];
      r = fwrite(mascota, sizeof(struct dogType), 1, ap);
   }
   if (r != 1  && cantidad-1 != 0){
      perror("No se pudo escribir");
      exit(-1);
   }
   r = fclose(ap);
   if ( r != 0){
      perror("No se pudo cerrar");
      exit(-1);
   }
   free(mascota);   // Se libera la memoria ocupada por la estructura
}

/* Metodo que cuenta cuantos registros hay en el archivo*/
int contarRegistro(){
   size_t r;
   FILE *ap;
   ap = fopen("dataDogs.dat", "r+");

    if (ap == NULL){ // TODO arreglar caso cuando el archivo no exite
      //perror("Error al abrir el archivo");
      //exit(-1);
      return 0;
   }
   int contador = 0;
   struct dogType *nueva;
   nueva = malloc(sizeof(struct dogType));   // Se separa memoria para la estructura
   while ( !feof(ap) ) {           // Ciclo que cuenta cuantas estructuras hay en el archivo
      r = fread(nueva, sizeof(struct dogType),1, ap);
      if (r =! 0){
          contador = contador + 1;
      } else {
        r = r+1;
      }
   }
   contador = contador - 1;
   if (r != 1){
     perror("No se pudo leer");
     exit(-1);
   }
   r = fclose(ap);
   if ( r != 0){
     perror("No se pudo cerrar");
     exit(-1);
   }
   free(nueva);   // Se libera la memoria ocupada por la estructura
   return contador;

}


/* Metodo que le envia al cliente la cantidad de registros actuales y le envia el registro solicitado*/
void verRegistro(int clientefd, char *ip){
   int cantidad = contarRegistro();
   enviarOpcion(clientefd, cantidad);

   if (cantidad != 0){ // En caso de que no haya registros no se reciben mas mensajes
      int opcion = recibirOpcion(clientefd); // Recibe el # de la mascota a buscar
      cargar(opcion, clientefd); // Llama al metodo cargar para que muestre encuentre la estructura deseada

      char *a;
      registrar(2, opcion, a, ip);
   }


}

/* Metodo que recibe una registro del cliente y la guarda*/
void ingresarRegistros(int clientefd, char *ip){
   struct dogType *mascota;
   mascota = malloc(sizeof(struct dogType));
   recibirMascota(clientefd, mascota);
   guardar(mascota);
   registrar( 1, 0, mascota-> nombre, ip );

   free(mascota);
}

/* Metodo que recibe el registro a eliminar dado por el cliente*/
void borrarRegistro(int clientefd, char *ip){
   int cantidad = contarRegistro(), opcion;
   enviarOpcion(clientefd, cantidad);
   if (cantidad != 0){ // En caso de que no haya registros no se reciben mas mensajes
      opcion = recibirOpcion(clientefd); // Recibe el # de la mascota a buscar
      borrarRegistroDelArchivo(opcion, cantidad); // Llama al metodo borrar

      char *a;
      registrar(3, opcion,a, ip ); // Se registra en el archivo .log
   }
}

/* Metodo que cambia a minuscula una cadena dada*/
void cambiarAMinuscula(char n[]){
   int j=0;
   for (j; j< strlen(n); j++){
      n[j] = tolower(n[j]);
   }
}

/* Metodo que busca todos los registros que contengan el nombre dado por el usuario */
int buscarRegistro1(int clientefd,struct wordType *pal, int cantidad){
   char c, aux[32];
   char nombreBuscado[32];
   strcpy(nombreBuscado, pal->palabra);
   cambiarAMinuscula(nombreBuscado);
   size_t r;
   FILE *ap;
   ap = fopen("dataDogs.dat", "r+");

   if (ap == NULL){
      perror("Error al abrir el archivo");
      exit(-1);
   }
   int encontrado=0, k=0;
   struct dogType *nueva;
   nueva = malloc(sizeof(struct dogType));
   for (k; k < cantidad; k++){
      r = fread(nueva, sizeof(struct dogType),1, ap);
      stpcpy(aux, nueva->nombre);
      cambiarAMinuscula(aux);
      if(  strcmp(nombreBuscado, aux) == 0  ){  // Compara el nombre buscado por el usuario con el nombre que esta en cada  estructura
          enviarOpcion(clientefd, 1);
          enviarMascota(clientefd, nueva);
          encontrado = 1;
      }
   }

   if (r != 1){
     perror("No se pudo leer");
     exit(-1);
   }
   r = fclose(ap);
   if ( r != 0){
     perror("No se pudo cerrar");
     exit(-1);
   }
   free(nueva);

   return encontrado;
}


/* Metodo que recibe la informacion de un registro para buscarlo*/
void buscarRegistro(int clientefd, char *ip){
   int cantidad = contarRegistro();
   enviarOpcion(clientefd, cantidad);
   if (cantidad != 0){
      struct wordType *pal;
      pal = malloc(sizeof(struct wordType));
      recibirPalabra(clientefd, pal);

      int encontrado = buscarRegistro1(clientefd, pal, cantidad);
      if (encontrado == 0  ){
         enviarOpcion(clientefd, -1); // Se indica que no se encontro la mascota
      } else {
         enviarOpcion(clientefd, 0); // Se indica que no se enviaran mas datos
      }


      registrar(4, 0, pal->palabra, ip);
      free(pal);
   }
}

/*
* Metodo que se utilizara para cada hilo
*/
void *atender(void *cli){
	struct cliType *cliente;
	cliente = cli;

	char *ip;
	ip  = cliente -> ip;
	int clientefd = cliente -> clienteFd;


	int opcionDelCliente = 0;
	// Ciclo para tomar peticiones hasta que el usuario seleccione 5
	do{
		int total=0, r1=0;
		int opcionDelCliente = recibirOpcion(clientefd);
		switch (opcionDelCliente){
		case 1:
		   ingresarRegistros(clientefd, ip);
		   break;
		case 2:
		   verRegistro(clientefd, ip);
		   break;
		case 3:
		   borrarRegistro(clientefd, ip);
		   break;
		case 4:
		   buscarRegistro(clientefd, ip);
		   break;
		default:
		   break;
		}

	} while (opcionDelCliente != 5); // 5 es para salir
	clientesActuales = clientesActuales - 1;

}


/**
* Main
*/

int main(){
   int r,clientefd;
   pthread_t hilos[BACKLOG];//Declaracion de los hilos que se van a utilizar
   struct sockaddr_in servidor, cliente;
   socklen_t socklen = 0;
   serverfd = socket(AF_INET,SOCK_STREAM,0); // crea un extremo de una comunicación y devuelve un descriptor.
   if(serverfd==-1){
      perror("Error al crear el socket\n");
      exit(-1);
   }

   // Señal para cuando se cierre el servidor con ctrl + c
   signal(SIGINT, salida);

   // Configuracion de estructura parametros del socket
   servidor.sin_family=AF_INET;
   servidor.sin_port=htons(PORT);
   servidor.sin_addr.s_addr = INADDR_ANY;
   bzero(servidor.sin_zero,8);

   // Configuracion del Socket
   r=bind(serverfd,(struct sockaddr*)&servidor,sizeof(struct sockaddr));
   if(r==-1){
      perror("Error en bind()\n");
      exit(-1);
   }
   // Define el numero maximo de usuarios que se atenderan
   r=listen(serverfd,BACKLOG);
   if(r==-1){
      perror("Error en listen()\n");
      exit(-1);
   }
   int run = 1;

   pid_t p; // Para crear el proceso

	struct cliType *cli;
	cli = malloc(sizeof( struct cliType ));


   while (run){
	printf("Servidor: entro en while(run) \n");
      if (clientesActuales < BACKLOG){
         clientefd=accept(serverfd,(struct sockaddr*)&cliente,&socklen);//inicializar socklen en 0
         if(clientefd==-1){
            perror("Error en accept()\n");
            exit(-1);
         }

	printf("Servidor: Acepto un cliente \n");
        /*  Se guardan los datos del cliente en un estructura*/
	char *ip;
	ip = inet_ntoa(cliente.sin_addr);
	cli-> ip = ip;
	cli->clienteFd = clientefd;
	/*****************************************************/

	r=pthread_create(&hilos[clientesActuales],NULL,(void*)atender,cli); //TODO corregir por que no siempre el valor para asignar el hilo es: clientesActuales. Igual funciona
	if (r != 0 ){
		perror("Error al crear el Hilo\n");
		exit(-1);
	}
	clientesActuales = clientesActuales + 1;

        /* p = fork();
	printf("Servidor: Creo un proceso\n");
         if (p == -1){
            perror("Error al crear el proceso");
            exit(-1);
         }
            if ( p!= 0){ // Es el padre
               clientesActuales = clientesActuales + 1; //TODO ubicar en el lugar correcto

            } else{ // Es el proceso hijo




 		// Menu del servidor
		atender(cli);
                close(clientefd);
            }*/
     }
   }
   close(serverfd);
}
