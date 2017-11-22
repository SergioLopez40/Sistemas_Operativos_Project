#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


struct dogType{
    char nombre[31];
	int edad;
	char raza[15];
	int estatura;
	float peso;
	char sexo[1];
};
struct dogType * perro;
int main(){
    //struct dogType * perro;
    struct dogType perro;
    // FILE para nombresMascotas.txt, razas.txt y sexo.txt
    FILE *archivo1,*archivo2,*archivo3,*fp;
    srand (time(NULL));

    // Se abre el archivo datDogs.dat
    fp = fopen ("datDogs.dat", "a");
    if (fp==NULL)
        {
        fputs ("File error",stderr); exit (1);
        }
    // Se abren los tres archivos
    archivo1= fopen("nombresMascotas.txt", "rt");
    archivo2= fopen("razas.txt", "rt");
    archivo3= fopen("sexo.txt", "rt");

    if(archivo1 == NULL)
    {
        printf("Error, el archivo nombresMascotas no se pudo abrir correctamente");
    }
    if(archivo2 == NULL)
    {
        printf("Error, el archivo razas no se pudo abrir correctamente");
    }
    if(archivo3 == NULL)
    {
        printf("Error, el archivo sexo no se pudo abrir correctamente");
    }

    // Se crean las estructuras
    for (int i = 0; i < 9999999; i++) {
        fseek(archivo1,0,SEEK_SET);


        int numero1 = rand () % 1717 + 1;
        int numero2 = rand () % 74 + 1;
        int numero3 = rand () % 2 + 1;

        signed int contador = 0;
        char * linea1;
        char * linea2;
        char * linea3;

    		//    Se toma un nombre de la lista en el archivo .txt de nombres 
    	linea1 = malloc(sizeof(char)*31);
        while(!feof(archivo1))
        {
            fgets(linea1, 31, archivo1);
            for(signed int i = 0; i <= strlen(linea1); i++)
            {
                if(linea1[i] == '\0')
                contador++;
            }
            if(contador == numero1)
            break;
        }
        linea1[strlen(linea1)-1]='\0';
        //perro.nombre=linea;
        strcpy(perro.nombre,linea1);
    	free(linea1);

    		//    Se toma una raza de la lista en el archivo .txt de razas
    	linea2 = malloc(sizeof(char)*16);
        //printf("raza%d\n",numero2);
        contador = 0;
        fseek(archivo2,0,SEEK_SET);
        while(!feof(archivo2))
        {
            fgets(linea2, 17, archivo2);
            for(signed int i = 0; i <= strlen(linea2); i++)
            {
                if(linea2[i] == '\0')
                contador++;
            }
            if(contador == numero2)
            break;
        }
        linea2[strlen(linea2)-1]='\0';
        strcpy(perro.raza,linea2);
    	free(linea2);

    		//    Se toma un sexo de la lista en el archivo .txt de sexos
    	linea3 = malloc(sizeof(char)*1);
        contador = 0;
        fseek(archivo3,0,SEEK_SET);
        while(!feof(archivo3))
        {
            fgets(linea3, 3, archivo3);
            for(signed int i = 0; i <= strlen(linea3); i++)
            {
                if(linea3[i] == '\0')
                contador++;
            }
            if(contador == numero3)
            break;
        }
        linea3[strlen(linea3)-1]='\0';
        strcpy(perro.sexo,linea3);
    	free(linea3);

    //  Da la edad aleatoria
        int edad;
        edad = rand () % 18;
        perro.edad=edad;

    //  Da una estatura aleatoria

    	int estatura;
        estatura = rand () % 100;
      	perro.estatura=estatura;

    //  Peso aleatorio

        float peso;
        peso = rand () % 40;
        perro.peso=peso;
        fwrite(&perro,sizeof(perro),1,fp);

    }
    fflush(fp);
    fclose(fp);
    fclose(archivo1);
    fclose(archivo2);
    fclose(archivo3);

    return 0;
}
