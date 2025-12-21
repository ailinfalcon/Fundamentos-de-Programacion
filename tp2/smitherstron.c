#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "reporte.h"
#include "incidentes.h"

#define MAX_DESCRIPCION 500
#define MAX_COMANDO 20
#define MAX_DATOS 550
#define MAX_INPUT 600

#define NORMAL "\e[0m"
#define ROJO_FUERTE "\e[31;1m"
#define BLANCO_FUERTE "\e[1m"

const char* CMDO_SALIR = "salir";
const int ERROR = -1;

/*
 * PRE: 'reporte' esta creado previamente con 'reporte_crear'.
 *      'argc' es una cántidad válida de argumentos.
 * POST: Si el archivo no se pudo abrir o no es válido (no está en orden o los datos son inválidos) devuelve false, caso contrario devuelve true.
 */
bool procesar_archivo(int argc, char * argv[], reporte_t * reporte, char descripcion_incidente[MAX_DESCRIPCION], int hora_incidente, int minutos_incidente, char dia_incidente, int prioridad_incidente){
    FILE * archivo_entrada = fopen(argv[1], "r");
    if(!archivo_entrada){
        printf(ROJO_FUERTE "Error al abrir el archivo.\n" NORMAL BLANCO_FUERTE);
        return false;
    }

    bool es_archivo_valido = leer_archivo_entrada(archivo_entrada, reporte, descripcion_incidente, &hora_incidente, &minutos_incidente, &dia_incidente, &prioridad_incidente);

    if(!es_archivo_valido){
        printf(BLANCO_FUERTE "El archivo es inválido\n");
        return false;
    }

    fclose(archivo_entrada);

    return true;
}

/*
 * PRE: 'reporte' está creado previamente con 'reporte_crear'.
 *      'datos' y 'comando' estan previamente inicializados con 'pedir_comandos'.
 *      'comando' es un string (termina en '\0') y es válido.
 * POST: Se ejecutan los comandos ingresados por el usuario hasta que se ingrese el comando 'salir'. 
 */
void procesar_comando(reporte_t * reporte, char input[MAX_INPUT], char datos[MAX_DATOS], char comando[MAX_COMANDO], int argc, char * argv[], char descripcion_inc[MAX_DESCRIPCION], int hora_inc, int min_inc, char dia_inc, int prioridad_inc){
    while(strcmp(comando, CMDO_SALIR) != 0){ 
        ejecutar_comando(comando, reporte, datos, descripcion_inc, hora_inc, min_inc, dia_inc, prioridad_inc, argc, argv);
        pedir_comandos(input, comando, datos);
    }

    if(strcmp(comando, CMDO_SALIR) == 0){
        ejecutar_comando(comando, reporte, datos, descripcion_inc, hora_inc, min_inc, dia_inc, prioridad_inc, argc, argv);
    }
}

int main(int argc, char * argv[]){
    char descripcion_incidente[MAX_DESCRIPCION];
    char dia_incidente = ' ';
    int hora_incidente = -1;
    int minutos_incidente = -1;
    int prioridad_incidente = 0;

    reporte_t * reporte;
    reporte = reporte_crear();
    if(reporte == NULL){
        return ERROR;
    }

    if(!es_cant_valida_argumentos(argc)){
        printf(ROJO_FUERTE "Cantidad inválida de argumentos.\n" NORMAL BLANCO_FUERTE);
        reporte_destruir(reporte);
        return ERROR;
    }

    bool se_proceso_archivo = procesar_archivo(argc, argv, reporte, descripcion_incidente, hora_incidente, minutos_incidente, dia_incidente, prioridad_incidente);

    if(!se_proceso_archivo){
        reporte_destruir(reporte);
        return ERROR;
    }

    char input[MAX_INPUT];
    char comando[MAX_COMANDO];
    char datos[MAX_DATOS];
    
    pedir_comandos(input, comando, datos);
    procesar_comando(reporte, input, datos, comando, argc, argv, descripcion_incidente, hora_incidente, minutos_incidente, dia_incidente, prioridad_incidente);

    reporte_destruir(reporte);

    return 0;
}   
