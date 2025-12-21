#ifndef __INCIDENTES_H__
#define __INCIDENTES_H__

#include <stdio.h>
#include "reporte.h"

#define MAX_INPUT 600
#define MAX_COMANDO 20
#define MAX_DATOS 550
#define MAX_DESCRIPCION 500

/*
 * PRE: -
 * POST: Devuelve true si la cantidad de argumentos es mayor o igual a 2, en caso contrario devuelve false.
 */
bool es_cant_valida_argumentos(int argc);

/*
 * PRE: 'input' debe estar inicializado con 'pedir_comandos'.
 * POST: Separa el input en comando y datos. 
 */
void buscar_comando(char input[MAX_INPUT], char comando[MAX_COMANDO], char datos[MAX_DATOS]);

/*
 * PRE: -
 * POST: Solicita el ingreso de un comando válido.
 */
void pedir_comandos(char input[MAX_INPUT], char comando[MAX_COMANDO], char datos[MAX_DATOS]);

/*
 * PRE:  'archivo_entrada' está previamente abierto con 'fopen'
 *       'reporte' está creado previamente con 'reporte_crear'
 * POST: Lee cada incidente del archivo recibido por parámetro, y los inserta en 'reporte'. Si algún incidente tiene datos inválidos o el archivo esta desordenado, devuelve false, en caso contrario, crea el incidente y lo inserta dentro del reporte. 
 */
bool leer_archivo_entrada(FILE * archivo_entrada, reporte_t * reporte, char descripcion_incidente[MAX_DESCRIPCION], int * hora_incidente, int * min_incidente, char * dia_incidente, int * prioridad_incidente);

/*
 * PRE: 'comando' es un comando válido.
 * POST: Ejecuta el procedimiento correspondiente al comando recibido. 
 */
void ejecutar_comando(char comando[MAX_COMANDO], reporte_t * reporte, char datos[MAX_DATOS], char descripcion[MAX_DESCRIPCION], int hora, int min, char dia, int prioridad, int argc, char * argv[]);


#endif /* __INCIDENTES_H__ */