#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//Estructura
struct dogType{

    char nombre[31];
	int edad;
	char raza[15];
	int estatura;
	float peso;
	char sexo[1];
	};
struct dogType * perro;

typedef struct nodo
{   
    struct dogType val;
    struct nodo *next, *before;
}Nodo;

struct list 
{   
    struct nodo *head, *tail;
    int size;

};

void crearLista( struct list *l){
    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
}


int hash(char *a){

     return (a[0]+27*a[1]+729*a[2])%1007;
    
}
void addL(struct list *l, struct dogType *val){
        Nodo *n;
        n = malloc(sizeof(Nodo));
        n->next = NULL;
        
        n->val = *val;
    if(l->size == 0){
        n->before = NULL;
        l->head = n;
        l->tail = n;
        l->size++;
        
        return;
    }
    Nodo *aux = l->tail;
    n->before = aux;
    aux->next = n;
    l->tail = n;
    l->size++;

}
void addF(struct list *l, struct dogType *val){
        Nodo *n;
        n = malloc(sizeof(Nodo));
        n->before = NULL;
        n->val = *val;
    if(l->size == 0){
        n->next = NULL;
        l->head = n;
        l->tail = n;
        l->size++;
        
        return;
    }
    Nodo *aux = l->head;
    n->next = aux;
    aux->before = n;
    l->head = n;
    l->size++;

}

Nodo *removeL (struct list *l){
    if (l->size < 1){
        return NULL;
    }else if (l->size == 1){
        Nodo *aux = l->head;
        crearLista(l);
        return aux;
    }
    Nodo *aux = l->tail;
    l->tail = aux->before;
    l->tail->next = NULL;
    l->size --;
    return aux;
}
struct list tablaHash[1007];

void addHash(struct dogType perro){
    addL(&tablaHash[hash(perro.nombre)], &perro);

}






//Menu principal
void imprimeMenuPrincipal()
    {
    system("clear");
  
    printf("     MENU PRINCIPAL      \n");
    printf("                         \n");
    printf("  1. Ingresar registro   \n");
    printf("  2. Ver registro        \n");
    printf("  3. Borrar registro     \n");
    printf("  4. Buscar registro     \n");
    printf("  5. Salir               \n");
   
    }


//Funcion menu
void opcionesMenu()
    {
    int opcion;
    imprimeMenuPrincipal();
    printf("\n Ingrese la opcion deseada: ");
    scanf ("%i", & opcion);

//Menu de opciones
    if (opcion==1)
	{
            printf("\n Ha seleccionado Ingresar registro\n \n");
            printf(" Desea continuar? Si(S) - No(N): ");
            char confirmacion;
            scanf ("%s", & confirmacion);
            if (confirmacion=='S'|| confirmacion=='s')
                {
                system("clear");
                struct dogType * perro;
                perro = malloc (sizeof(struct dogType));
                ingresarRegistro(perro);
                free(perro);
                exit(0);
                }
            else if (confirmacion=='N' || confirmacion=='n')
                {
                system("clear");
                opcionesMenu();
                }
            }

      else if (opcion== 2)
            {
            printf("\n Ha seleccionado Ver registro\n \n");
            printf(" Desea continuar? Si(S) - No(N): ");
            char confirmacion;
            scanf ("%s", & confirmacion);
            if (confirmacion=='S'|| confirmacion=='s')
                {
                system("clear");
                struct dogType * perro;
                perro = malloc (sizeof(struct dogType));
                verRegistro(perro);
                free(perro);
                exit(0);
                }
            else if (confirmacion=='N' || confirmacion=='n')
                {
                system("clear");
                opcionesMenu();
                }
            }

        else if (opcion== 3)
            {
            printf("\n Ha seleccionado Borrar registro\n \n");
            printf(" Desea continuar? Si(S) - No(N): ");
            char confirmacion;
            scanf ("%s", & confirmacion);
            if (confirmacion=='S'|| confirmacion=='s')
                {
                system("clear");
                borrarRegistro();
                exit(0);
                }
            else if (confirmacion=='N' || confirmacion=='n')
                {
                system("clear");
                opcionesMenu();
                }
            }

        else if (opcion== 4)
            {
            printf("\n Ha seleccionado Buscar registro\n \n");
            printf(" Desea continuar? Si(S) - No (N): ");
            char confirmacion;
            scanf ("%s", & confirmacion);
            if (confirmacion=='S'|| confirmacion=='s')
                {
                system("clear");
                buscarRegistro(perro);
               exit(0);
                }
            else if (confirmacion=='N' || confirmacion=='n')
                {
                system("clear");
                opcionesMenu();
                }
            }

        else if (opcion== 5)
            {
            printf("\n Ha seleccionado Salir\n \n");
            printf(" Desea salir? Si(S) - No(N): ");
            char confirmacion;
            scanf ("%s", & confirmacion);
            if (confirmacion=='S'|| confirmacion=='s')
                {
                exit(0);
                }
            else if (confirmacion=='N' || confirmacion=='n')
                {
                system("clear");
                opcionesMenu();
                }
            }
          else
            {
                system("clear");
                opcionesMenu();
            }
        }
    


// 1 Ingresar registro 
ingresarRegistro (void * dt)
    {
    FILE *fp;
    fp = fopen ("datDogs.dat", "a");
    if (fp==NULL)
        {
        fputs ("File error",stderr); exit (1);
        }
    struct dogType perro;
    printf("\n  Ingrese el nombre: ");
    scanf("%s",perro.nombre);
    printf("  Ingrese la edad: ");
    scanf("%i",&perro.edad);
    printf("  Ingrese la raza: ");
    scanf("%s", perro.raza);
    printf("  Ingrese la estatura: ");
    scanf("%i", &perro.estatura);
    printf("  Ingrese el peso: ");
    scanf("%f", &perro.peso);
    printf("  Ingrese el sexo: ");
    scanf("%s", perro.sexo);
    fwrite(&perro,sizeof(perro),1,fp);
    fclose(fp);
    printf(" \n Desea continuar? Si(S) - No(N): ");
    char confirmacion;
    scanf ("%s", & confirmacion);
    if (confirmacion=='S'|| confirmacion=='s')
        {
        system("clear");
        opcionesMenu();
        }
    else if (confirmacion=='N' || confirmacion=='n')
        {
        exit(0);
        }
    }


// 2 Ver registro
verRegistro (void * dt)
    {
    FILE *fichero;
    fichero = fopen( "datDogs.dat", "r" );
    fseek(fichero, 0, SEEK_END);
    int numRegistros = ftell(fichero)/sizeof(struct dogType);
    int x;
    printf("\n  Existen %d registros",numRegistros);
    int consulta;
    printf(" \n \n Escriba el numero del registro que desea consultar: ");
    
    scanf ("%i", & consulta);
    char historia[15];
    sprintf(historia,"%i.txt",consulta);
    FILE* texto=fopen(historia, "r");
    if (texto==NULL){
    
    
   
    
    fseek (fichero, (consulta-1)*sizeof(struct dogType), SEEK_SET);
    struct dogType perro;
    fread(&perro,sizeof(struct dogType),1,fichero);
    texto=fopen(historia,"a");
    
    fprintf(texto, "*************HISTORIA CLINICA**************** \n\n\n\n",consulta);
         fprintf(texto, "NOMBRE:   %s\n", perro.nombre);
         fprintf(texto, "EDAD:     %i\n", perro.edad);
         fprintf(texto, "RAZA:     %s\n", perro.raza);
         fprintf(texto, "ESTATURA: %i\n", perro.estatura);
         fprintf(texto, "PESO:     %f\n", perro.peso);
         fprintf(texto, "SEXO:     %s\n", perro.sexo);
         fprintf(texto, "No.HISTORIA: %i\n\n\n", consulta);
         fprintf(texto, "*************COMENTARIOS VETERINARIA************ \n",consulta);
         fclose(texto);
         fclose( fichero );
 
}
    char  reg[32];
    char sys[32];
    sprintf(reg,"%i",consulta);
    strcat(reg,".txt");
    strcpy(sys,"gedit ");
    strcat(sys,reg);
    system(sys);


        printf("\n Desea continuar? Si(S) - No(N): ");
    char confirmacion;
    scanf ("%s", & confirmacion);
    if (confirmacion=='S'|| confirmacion=='s')
        {
        system("clear");
        opcionesMenu();
        }
    else if (confirmacion=='N' || confirmacion=='n')
        {
        exit(0);
        }
    }


// 3 Borrar registro
borrarRegistro (void * dt)
    {
    FILE *fichero;
    FILE *fichero2;
    struct dogType perro;
    fichero = fopen( "datDogs.dat", "r" );
    fseek(fichero, 0, SEEK_END);
    int numRegistros = ftell(fichero)/sizeof(perro);
    int x;
    for (x=1; x<=numRegistros; x++)
        {
        printf("\n  * Registro # %i",x);
        }
    int consulta;
    printf(" \n \n Escriba el numero del registro que desea eliminar: ");
    scanf ("%i", & consulta);
    fseek (fichero, (consulta-1)*sizeof(perro), SEEK_SET);
    fread (&perro, sizeof(perro), 1, fichero);
    printf( "\n Nombre: %s\n", perro.nombre );
    printf( " Edad: %i\n", perro.edad);
    printf( " Raza: %s\n", perro.raza);
    printf( " Estatura: %i\n", perro.estatura);
    printf( " Peso: %f\n", perro.peso);
    printf( " Sexo: %s\n", perro.sexo);
    fichero2 = fopen( "datDogsaux.dat", "a" );
    int w=0;
    for (w;w<numRegistros;w++)
        {
        if (consulta-1!=w)
            {
            fseek (fichero, w*sizeof(perro), SEEK_SET);
            fread (&perro, sizeof(perro), 1, fichero);
            fwrite(&perro,sizeof(perro),1,fichero2);
            }
        }
    fclose( fichero );
    fclose( fichero2 );
    if(remove("datDogs.dat")==0)
        {
        printf("\n El registro fue eliminado satisfactoriamente\n");
        }
    else
        {
        printf("No se pudo eliminar el archivo\n");
        }
    rename("datDogsaux.dat","datDogs.dat")==0;

    char aux [32];
    sprintf(aux,"%i", consulta);
    strcat(aux,".txt");
    FILE* fichero3;
    fichero3=fopen(aux,"r");
    printf("\n Ademas desea borrar la historia clinica Si(S) - No(N):" );
    char con;
    scanf("%s", &con);
    if(con=='S'||con=='s'){
    if(fichero3 == NULL){
        printf( "\n La historia clinica no existia\n" );
    }else{
        remove(aux);
        printf( "\n La historia clinica fue borrada exitosamente\n" );
    }
    }else if (con=='n'||con=='N'){
        printf("\n No se borrara la historia clinica\n" );
    }
    
    

    printf("\n Desea continuar? Si(S) - No(N): ");
    char confirmacion;
    scanf ("%s", & confirmacion);
    if (confirmacion=='S'|| confirmacion=='s')
        {
        system("clear");
        opcionesMenu();
        }
    else if (confirmacion=='N' || confirmacion=='n')
        {
        exit(0);
        }
    }




// 4 Buscar registro 
buscarRegistro (void * dt)
    {
FILE *fichero;
    struct dogType perro;
    fichero = fopen( "datDogs.dat", "r" );
    fseek(fichero, 0, SEEK_END);
    int numRegistros = ftell(fichero)/sizeof(perro);
    char consulta[31];
    printf(" \n \n Escriba el nombre del perro que desea consultar: ");
    scanf ("%s", consulta);
    int i=0;
    int busqueda=0;
    char nombre[31];
    for (i; i<numRegistros;i++)
        {
        fseek (fichero, i*sizeof(perro), SEEK_SET);
        fread (&perro, sizeof(perro), 1, fichero);
        minisculasMayusculas(consulta);
        char nombre[31];
        strcpy (nombre,perro.nombre);
        minisculasMayusculas(nombre);
        if( (strcmp(nombre,consulta) == 0) )
            {
            printf( "\n Nombre: %s\n", perro.nombre );
            printf( " Edad: %i\n", perro.edad);
            printf( " Raza: %s\n", perro.raza);
            printf( " Estatura: %i\n", perro.estatura);
            printf( " Peso: %f\n", perro.peso);
            printf( " Sexo: %s\n", perro.sexo);
            busqueda=1;
            }
        }
        if (busqueda==0)
        {
        printf ("\n !Registro no encontrado! \n");
        }
    fclose( fichero );
        printf("\n Desea continuar? Si(S) - No(N): ");
    char confirmacion;
    scanf ("%s", & confirmacion);
    if (confirmacion=='S'|| confirmacion=='s')
        {
        system("clear");
        opcionesMenu();
        }
    else if (confirmacion=='N' || confirmacion=='n')
        {
        exit(0);
        }
}

void minisculasMayusculas(char s[])
{
	int i=0;
	int desp='a'-'A';
	for (i=0;s[i]!='\0';++i)
	{
		if(s[i]>='a'&&s[i]<='z')
		{
			s[i]=s[i]-desp;
		}
	}
}

int main()
    {
    int opcion=0;
    opcionesMenu(opcion);
    return 0;
    }
 

