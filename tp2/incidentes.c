#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "incidentes.h"
#include "reporte.h"

#define MAX_COMANDO 20
#define MAX_DATOS 550
#define MAX_DESCRIPCION 500
#define MAX_INPUT 600
#define MAX_CONTADORES 7
#define MAX_HORA_COMPLETA 5
#define MAX_HORA_STR 2
#define MAX_MIN_STR  2
#define MAX_PRIORIDAD_STR 3
#define MAX_INCIDENTES 20
#define MAX_DIAS 7
#define ERROR -1

#define POS_LUNES 0
#define POS_MARTES 1
#define POS_MIERCOLES 2
#define POS_JUEVES 3
#define POS_VIERNES 4
#define POS_SABADO 5
#define POS_DOMINGO 6

#define LUNES_MAYUS  'L'
#define MARTES_MAYUS  'M'
#define MIERCOLES_MAYUS  'X'
#define JUEVES_MAYUS  'J'
#define VIERNES_MAYUS  'V'
#define SABADO_MAYUS  'S'
#define DOMINGO_MAYUS 'D'

#define NORMAL "\e[0m"
#define SUBRAYADO "\e[4m"
#define ROJO_FUERTE "\e[31;1m"
#define AZUL_FUERTE "\e[34;1m"
#define BLANCO_FUERTE "\e[1m"
#define VERDE_FUERTE "\e[32;1m"


const char* FORMATO_LECTURA_ARCHIVO = "%[^;];%d:%d;%c;%i\n";
const char* FORMATO_LECTURA_INPUT = " %[^\n]";
const char* FORMATO_LECTURA_INGRESO = "  %[^;];%d:%d;%c;%i";
const char* DIAS_SEMANA = "LMXJVSD";
const char* NOMBRES_DIAS[MAX_DIAS] = {"LUNES", "MARTES", "MIÉRCOLES", "JUEVES", "VIERNES", "SÁBADO", "DOMINGO"};
const char* COMANDO_ASIGNAR = "asignar_incidente";
const char* COMANDO_AGREGAR = "agregar_incidente";
const char* COMANDO_IMPRIMIR = "imprimir_incidentes";
const char* COMANDO_MULTIPLES_INCIDENTES = "agregar_multiples_incidentes";
const char* COMANDO_DIA_ACCIDENTADO = "dia_mas_accidentado";
const char* COMANDO_SALIR = "salir";

const int CANT_DATOS_INCIDENTES = 5;
const int CANT_MIN_ARGUMENTOS = 2;
const int MINIMA_PRIORIDAD = 1;
const int MAXIMA_PRIORIDAD = 100;
const int MINIMA_HORA = 0;
const int MAXIMA_HORA = 23;
const int MINIMO_MINUTOS = 0;
const int MAXIMO_MINUTOS = 59;
const int MAXIMO_ARGUMENTOS = 3;
const int CONTADOR_VACIO = 0;
const int NO_HAY_DIA_ACCIDENTADO = -1;
const int SIN_INCIDENTES = 0;

/*
 * PRE: -
 * POST: Devuelve true si la cantidad de argumentos es mayor o igual a 2, en caso contrario devuelve false.
 */
bool es_cant_valida_argumentos(int argc){
    return argc >= CANT_MIN_ARGUMENTOS;
}

/*
 * PRE: 'descripcion' es un string (termina en '\0').
 * POST: Devuelve true si la descripción es menor o igual a la cantidad máxima de carácteres, en caso contrario devuelve false.
 */
bool es_cant_descripcion_valida(char descripcion[MAX_DESCRIPCION]){
    return (strlen(descripcion) > 0 && strlen(descripcion) <= MAX_DESCRIPCION);
}

/*
 * PRE: -
 * POST: Devuelve true si es un dia válido, false en caso contrario.
 */
bool es_dia_valido(char dia){
    bool dia_valido = false;
    size_t tope_dias = strlen(DIAS_SEMANA);

    for(size_t i = 0; i < tope_dias; i++){
        if(DIAS_SEMANA[i] == dia){
            dia_valido = true;
        }
    }

    return dia_valido;
}

/*
 * PRE: -
 * POST: Devuelve true si la prioridad es mayor o igual a 1 y menor o igual a 100, en caso contrario devuelve false.
 */
bool es_prioridad_valida(int prioridad){
    return (prioridad >= MINIMA_PRIORIDAD && prioridad <= MAXIMA_PRIORIDAD);
}

/*
 * PRE: -
 * POST: Devuelve true si la hora es en formato 24 horas, caso contrario devuelve false.
 */
bool es_hora_valida(int hora, int minutos){
    return ((hora >= MINIMA_HORA && hora <= MAXIMA_HORA) && (minutos >= MINIMO_MINUTOS && minutos <= MAXIMO_MINUTOS));
}

/*
 * PRE: -
 * POST: Devuelve true si el archivo esta ordenado por hora ascendente, en caso contrario, devuelve false.
 */
bool esta_ordenado_por_hora(int hora_actual, int hora_anterior, int min_actual, int min_anterior){
    if(hora_anterior < hora_actual){
        return true;
    } else if (hora_anterior == hora_actual && min_anterior <= min_actual){
        return true;
    }

    return false;
}

/*
 * PRE: -
 * POST: Devuelve true si el archivo esta ordenado por prioridad, en caso contrario, devuelve false.
 */
bool esta_ordenado_por_prioridad(int prioridad_anterior, int prioridad_actual, int hora_anterior, int hora_actual, int min_anterior, int min_actual){
    if(prioridad_anterior < prioridad_actual){
        return true;
    } else if(prioridad_anterior == prioridad_actual){
        if(esta_ordenado_por_hora(hora_actual, hora_anterior, min_actual, min_anterior)){
            return true;
        }
    }

    return false;
}


/*
 * PRE: -
 * POST: Devuelve true si los datos del incidente son válidos y esta ordenado por prioridad, en caso contrario devuelve false.
 */
bool es_incidente_valido(char descripcion[MAX_DESCRIPCION], int hora, int min, char dia, int prioridad, bool primer_incidente, int hora_ant, int min_ant, int prioridad_anterior){
    if(!es_cant_descripcion_valida(descripcion)){
        return false;
    }
    
    if(!es_hora_valida(hora, min)){
        return false;
    } 
    
    if(!es_dia_valido(dia)){
        return false;
    }

    if(!es_prioridad_valida(prioridad)){
        return false;
    }

    if(!primer_incidente){
        if(!esta_ordenado_por_prioridad(prioridad_anterior, prioridad, hora_ant, hora, min_ant, min)){
            return false;
        }
    }

    return true;
}


/*
 * PRE: 'comando' es un string (termina en \0).
 * POST: Devuelve true si el comando es válido, o false en caso contrario.
 */
bool es_comando_valido(char comando[MAX_COMANDO]){
    return (strcmp(comando, COMANDO_AGREGAR) == 0 || strcmp(comando, COMANDO_ASIGNAR) == 0 || strcmp (comando, COMANDO_DIA_ACCIDENTADO) == 0 || strcmp(comando, COMANDO_IMPRIMIR) == 0 || strcmp(comando, COMANDO_MULTIPLES_INCIDENTES) == 0 || strcmp(comando, COMANDO_SALIR) == 0);
}

/*
 * PRE: -
 * POST: Muestra por pantalla una lista de comandos válidos.
 */
void mostrar_comandos(){
    printf(AZUL_FUERTE "\n> " NORMAL BLANCO_FUERTE "Ingrese un comando para continuar:\n");
    printf("  . %s\n", COMANDO_ASIGNAR);
    printf("  . %s <descripcion>;<hora>;<dia>;<prioridad>\n", COMANDO_AGREGAR);
    printf("  . %s\n", COMANDO_IMPRIMIR);
    printf("  . %s <n>\n", COMANDO_MULTIPLES_INCIDENTES);
    printf("  . %s\n", COMANDO_DIA_ACCIDENTADO);
    printf("  . %s\n\n", COMANDO_SALIR);
    printf(AZUL_FUERTE "\n> " NORMAL BLANCO_FUERTE);
}

/*
 * PRE: 'i' es una posición válida dentro de 'input', es decir mayor o igual a 0 o menor estricto a 'MAX_INPUT'
 * POST: Separa el comando dentro de 'input' y lo guarda en 'comando'.
 */
void separar_comando_input(char input[MAX_INPUT], char comando[MAX_COMANDO], int * i){
    int j = 0;

    while(input[*i] != ' ' && input[*i] != '\0'){
        comando[j] = input[*i];
        j++;
        (*i)++;
    }

    comando[j] = '\0';
    (*i)++;
}

/*
 * PRE: 'i' es una posición válida dentro de 'input', es decir mayor o igual a 0 o menor estricto  a 'MAX_INPUT'
 * POST: Separa los datos dentro de 'input' despues del primer espacio encontrado. Si no hay espacio, 'datos' queda como cadena vacía.
 */
void separar_datos_input(char input[MAX_INPUT], char datos[MAX_DATOS], int * i){
    int j = 0;

    while(input[*i] != '\0'){
        datos[j] = input[*i];
        (*i)++;
        j++;
    }
    
    datos[j] = '\0';
}

/*
 * PRE: 'input' debe estar inicializado con 'pedir_comandos'.
 * POST: Separa el input en comando y datos. 
 */
void buscar_comando(char input[MAX_INPUT], char comando[MAX_COMANDO], char datos[MAX_DATOS]){
    int i = 0;

    separar_comando_input(input, comando, &i);
    separar_datos_input(input, datos, &i);
}

/*
 * PRE: 'i' debe ser una posicion válida dentro de 'hora_completa', es decir, mayor o igual a 0 o menor estricto a 'MAX_HORA_COMPLETA'.
 * POST: Separa la hora del incidente dentro de 'hora_completa', la guarda en 'hora_str' para convertirla en int y guardarla en 'hora'. Si la posición siguiente a 'hora_str' es ':', devuelve true, en caso contrario, devuelve false.
 */
bool separar_hora_int(char hora_completa[MAX_HORA_COMPLETA], int * hora, int * i){
    char hora_str[MAX_HORA_STR];
    int k = 0;

    while(hora_completa[*i] != ':' && hora_completa[*i] != '\0' && k < MAX_HORA_STR){
        hora_str[k] = hora_completa[*i];
        k++;
        (*i)++;
    }
    hora_str[k] = '\0';
    (*hora) = atoi(hora_str);

    if(hora_completa[*i] != ':'){
        return false;
    }
    (*i)++;

    return true;
}

/*
 * PRE: 'i' debe ser una posicion válida dentro de 'hora_completa', es decir, mayor o igual a 0 o menor estricto a 'MAX_HORA_COMPLETA'
 * POST: Separa los minutos del incidente dentro de 'hora_completa', los convierte en int y los guarda en 'min'.
 */
void separar_min_int(char hora_completa[MAX_HORA_COMPLETA], int * min, int * i){
    char min_str[MAX_MIN_STR];
    int k = 0;

    while(hora_completa[*i] != '\0' && k < MAX_MIN_STR){
        min_str[k] = hora_completa[*i];
        k++;
        (*i)++;
    }

    min_str[k] = '\0';
    (*min) = atoi(min_str);
}

/*
 * PRE: 'hora_completa' debe estar inicializada con 'separar_hora_completa'.
 * POST: Devuelve true si el formato es 'HH:MM' y guarda en 'hora' y 'min' la hora y los minutos como enteros, caso contrario devuelve false.
 */
bool separar_hora(char hora_completa[MAX_HORA_COMPLETA], int * hora, int * min){
    int i = 0;

    if(!separar_hora_int(hora_completa, hora, &i)){
        return false;
    }

    separar_min_int(hora_completa, min, &i);

    return true;
}

/*
 * PRE: 'i' debe ser una posicion válida dentro de 'datos', es decir, mayor o igual a 0 o menor estricto a 'MAX_DATOS'
 * POST: Separa la descripción del incidente dentro de 'datos', y lo guarda en 'descripcion'. Si la posición siguiente a descripción contiene ';' devuelve true, en caso contrario, devuelve false.
 */
bool separar_descripcion(char datos[MAX_DATOS], char descripcion[MAX_DESCRIPCION], int * i){
    int j = 0;

    while(datos[*i] != ';' && datos[*i] != '\0'){
        descripcion[j] = datos[*i];
        (*i)++;
        j++;
    }
    descripcion[j] = '\0';
    
    if(datos[(*i)] != ';'){
        return false;
    }

    (*i)++;

    return true;
}

/*
 * PRE: 'i' debe ser una posicion válida dentro de 'datos', es decir, mayor o igual a 0 o menor estricto a 'MAX_DATOS'
 * POST: Separa la hora completa del incidente dentro de 'datos', la guarda en 'hora_completa' para despúes separarla en hora y minutos y convertirla en int. Si la posición siguiente a la hora completa, contiene ';' devuelve true, en caso contrario, devuelve false.
 */
bool separar_hora_completa(char datos[MAX_DATOS], int * hora, int * minutos, int * i){
    int j = 0;

    char hora_completa[MAX_HORA_COMPLETA];
    while(datos[*i] != ';' && datos[*i] != '\0' && j < MAX_HORA_COMPLETA){
        hora_completa[j] = datos[*i];
        (*i)++;
        j++;
    }
    hora_completa[j] = '\0';
    
    if(datos[*i] != ';'){
        return false;
    }
    (*i)++;

    (*hora) = 0;
    (*minutos) = 0;

    if(!separar_hora(hora_completa, hora, minutos)){
        printf("\nEl formato de la hora es inválido\n\n");
        return false;
    }

    return true;
}

/*
 * PRE: 'i' debe ser una posicion válida dentro de 'datos', es decir, mayor o igual a 0 o menor estricto a 'MAX_DATOS'
 * POST: Separa el dia del incidente dentro de 'datos', y lo guarda en 'dia'. Si la posición siguiente al dia, contiene ';' devuelve true, en caso contrario, devuelve false.
 */
bool separar_dia(char datos[MAX_DATOS], char * dia, int * i){
    if(datos[*i] != '\0'){
        (*dia) = datos[*i];
        (*i)++;
    }
    
    if(datos[*i] != ';'){
        return false;
    }
    (*i)++;

    return true;
}

/*
 * PRE: 'i' debe ser una posicion válida dentro de 'datos', es decir, mayor o igual a 0 o menor estricto a 'MAX_DATOS'
 * POST: Separa la prioridad del incidente dentro de 'datos', la convierte en int y la guarda en 'prioridad'.
 */
void separar_prioridad(char datos[MAX_DATOS], int * prioridad, int * i){
    int j = 0;

    char prioridad_str[MAX_PRIORIDAD_STR];
    (*prioridad) = 0;

    while(datos[*i] != '\0' && j < MAX_PRIORIDAD_STR){
        prioridad_str[j] = datos[*i];
        (*i)++;
        j++;
    }
    prioridad_str[j] = '\0';
    (*prioridad) = atoi(prioridad_str);
}

/*
 * PRE: 'datos' debe estar inicializado con 'buscar_comando'.
 * POST: Si el formato es válido, devuelve true y separa los datos recibidos en descripcion, hora, minutos, dia y prioridad, en caso contrario devuelve false.
 */
bool separar_datos(char datos[MAX_DATOS], char descripcion[MAX_DESCRIPCION], int *hora, int *minutos, char * dia, int *prioridad){
    int i = 0;
    
    if(!separar_descripcion(datos, descripcion, &i)){
        return false;
    }

    if(!separar_hora_completa(datos, hora, minutos, &i)){
        return false;
    }

    if(!separar_dia(datos, dia, &i)){
        return false;
    }

    separar_prioridad(datos, prioridad, &i);

    return true;
}

/*
 * PRE: -
 * POST: Solicita el ingreso de un comando válido.
 */
void pedir_comandos(char input[MAX_INPUT], char comando[MAX_COMANDO], char datos[MAX_DATOS]){
    mostrar_comandos();
    scanf(FORMATO_LECTURA_INPUT, input);
    buscar_comando(input, comando, datos);

    while(!es_comando_valido(comando)){
        printf(ROJO_FUERTE "\nEl comando es inválido\n\n");
        mostrar_comandos();
        scanf(FORMATO_LECTURA_INPUT, input);
        buscar_comando(input, comando, datos);
    }
}

/*
 * PRE: 'incidente' debe estar previamente cargado con 'crear_incidente'.
 * POST: Imprime por pantalla los datos del incidente.
 */
void mostrar_info_incidente(incidente_t incidente){
    int hora = incidente.minutos_desde_medianoche / 60;
    int minutos = incidente.minutos_desde_medianoche % 60;

    printf(AZUL_FUERTE "\n. " NORMAL "%s\n", incidente.descripcion);
    printf("HORA: %02d:%02d\n", hora, minutos);
    printf("DIA DEL INCIDENTE: %c\n", incidente.dia);
    printf("PRIORIDAD: %i\n", incidente.prioridad);
}

/*
 * PRE: 'reporte' debe estar previamente creado con 'reporte_crear'.
 * POST: Imprime por pantalla los detalles del siguiente incidente a asignar, y lo elimina del reporte.
 */
void ejecutar_comando_asignar_incidente(reporte_t* reporte){
    incidente_t  incidente_eliminado;

    if(!eliminar_incidente(reporte, &incidente_eliminado)){
        printf( VERDE_FUERTE "\nNo hay incidentes para asignar!\n\n");
    } else {
        printf(AZUL_FUERTE "\nINCIDENTE A ASIGNAR: \n");
        mostrar_info_incidente(incidente_eliminado);
        free(incidente_eliminado.descripcion);
    }
}

/*
 * PRE: 'hora' es válida.
 *      'minutos' es válida.
 * POST: Devuelve la hora en minutos.
 */
int calcular_minutos_incidente(int hora, int minutos){
    return (hora*60 + minutos);
}

/*
 * PRE: La longitud de 'descripcion' debe ser mayor estricto a 0 y menor o igual a 500.
 *      'hora' debe ser mayor o igual a 0 y menor o igual a 23.
 *      'minutos' debe ser mayor o igual a 0 y menor o igual a 59.
 *      'dia' debe estar en mayúscula, y ser un dia válido.
 *      'prioridad' debe ser mayor o igual a 1 y menor o igual a 100.
 * POST: Crea un incidente con datos válidos.
 */
void crear_incidente(incidente_t * incidente, char descripcion[MAX_DESCRIPCION], int hora, int minutos, char dia, int prioridad){
    strcpy(incidente->descripcion, descripcion);
    incidente->minutos_desde_medianoche = calcular_minutos_incidente(hora, minutos);
    incidente->dia = dia;
    incidente->prioridad = prioridad;
}


/*
 * PRE: 'reporte_aux' y 'reporte_ordenado' estan previamente creados con 'reporte_crear'.
 *      'incidente_eliminado' e 'incidente' estan declarados previamente.
 * POST: Inserta 'incidente_eliminado' en 'reporte_ordenado' si su horario es menor al de 'incidente', en caso contrario lo inserta en 'reporte_aux'. 
 */
void ordenar_incidente_por_min(reporte_t * reporte_aux, reporte_t * reporte_ordenado, incidente_t incidente_eliminado, incidente_t incidente){
    if(incidente_eliminado.minutos_desde_medianoche < incidente.minutos_desde_medianoche){
        agregar_incidente(reporte_ordenado, incidente_eliminado);
        free(incidente_eliminado.descripcion);
    } else {
        agregar_incidente(reporte_aux, incidente_eliminado);
        free(incidente_eliminado.descripcion);
    }
}

/*
 * PRE: 'reporte_aux' y 'reporte_ordenado' estan previamente creados con 'reporte_crear'.
 *      'incidente_eliminado' e 'incidente' estan declarados previamente.
 * POST: Inserta 'incidente_eliminado' en 'reporte_ordenado' si su prioridad es menor al de 'incidente', en caso contrario lo inserta en 'reporte_aux'. Si las prioridades son iguales, lo ordena por minutos desde la medianoche.
 */
void ordenar_por_prioridad(reporte_t * reporte_ordenado, reporte_t * reporte_aux, incidente_t incidente_eliminado, incidente_t incidente){
    if(incidente_eliminado.prioridad < incidente.prioridad){
        agregar_incidente(reporte_ordenado, incidente_eliminado);
        free(incidente_eliminado.descripcion);
    } else if(incidente_eliminado.prioridad == incidente.prioridad){
        ordenar_incidente_por_min(reporte_aux, reporte_ordenado, incidente_eliminado, incidente);
    } else {
        agregar_incidente(reporte_aux, incidente_eliminado);
        free(incidente_eliminado.descripcion);
    }
}

/*
 * PRE: 'reporte_origen' y 'reporte_dest' deben estar previamente creados con 'reporte_crear'
 * POST: Inserta todos los incidentes de 'reporte_origen' en 'reporte_dest'.
 */
void mover_incidentes(reporte_t * reporte_origen, reporte_t * reporte_dest, incidente_t incidente){
    while(eliminar_incidente(reporte_origen, &incidente)){
        agregar_incidente(reporte_dest, incidente);
        free(incidente.descripcion);
    }
}

/*
 * PRE: 'reporte' está previamente creado con 'reporte_crear'
 *      'incidente' está creado previamente.
 * POST: Inserta el incidente dentro de 'reporte' en la posición correcta, respetando el orden de prioridad.
 */
void ordenar_incidente(reporte_t * reporte, incidente_t incidente){
    reporte_t * reporte_ordenado;
    reporte_ordenado = reporte_crear();
    if(reporte_ordenado == NULL){
        return;
    }

    reporte_t * reporte_aux;
    reporte_aux = reporte_crear();
    if(reporte_aux == NULL){
        return;
    }

    incidente_t incidente_eliminado;

    while(eliminar_incidente(reporte, &incidente_eliminado)){
        ordenar_por_prioridad(reporte_ordenado, reporte_aux, incidente_eliminado, incidente);
    }

    agregar_incidente(reporte_ordenado, incidente);
    free(incidente.descripcion);
    
    mover_incidentes(reporte_aux, reporte_ordenado, incidente_eliminado);
    mover_incidentes(reporte_ordenado, reporte, incidente_eliminado);

    reporte_destruir(reporte_aux);
    reporte_destruir(reporte_ordenado);
}

/*
 * PRE: 'descripcion' es un dato válido.
 *      'hora' es un dato válido.
 *      'minutos' es un dato válido.
 *      'dia' es un dato válido.
 *      'prioridad' es un dato válido.
 * POST: Crea el incidente, y lo inserta dentro de 'reporte' manteniendo el orden de prioridad.
 */
void insertar_incidente(reporte_t * reporte, char descripcion[MAX_DESCRIPCION], int hora, int minutos, char dia, int prioridad){
    incidente_t incidente;
    incidente.descripcion = malloc(sizeof(char)*MAX_DESCRIPCION);

    crear_incidente(&incidente, descripcion, hora, minutos, dia, prioridad);
    ordenar_incidente(reporte, incidente);
}

/*
 * PRE:  'archivo_entrada' está previamente abierto con 'fopen'
 *       'reporte' está creado previamente con 'reporte_crear'
 * POST: Lee cada incidente del archivo recibido por parámetro, y los inserta en 'reporte'. Si algún incidente tiene datos inválidos o el archivo esta desordenado, devuelve false, en caso contrario, crea el incidente y lo inserta dentro del reporte. 
 */
bool leer_archivo_entrada(FILE * archivo_entrada, reporte_t * reporte, char descripcion_incidente[MAX_DESCRIPCION], int * hora_incidente, int * min_incidente, char * dia_incidente, int * prioridad_incidente){
    bool archivo_valido = true;
    int prioridad_anterior = 0;
    int hora_ant = -1;
    int min_ant = -1;

    int incidente_leido = fscanf(archivo_entrada, FORMATO_LECTURA_ARCHIVO, descripcion_incidente, hora_incidente, min_incidente, dia_incidente, prioridad_incidente);
    bool primer_incidente = true;

    while(incidente_leido != EOF && incidente_leido == CANT_DATOS_INCIDENTES){
        if(!es_incidente_valido(descripcion_incidente, *hora_incidente, *min_incidente, *dia_incidente, *prioridad_incidente, primer_incidente, hora_ant, min_ant, prioridad_anterior)){
            archivo_valido = false;
        }

        prioridad_anterior = *prioridad_incidente;
        hora_ant = *hora_incidente;
        min_ant = *min_incidente;
        primer_incidente = false;
            
        insertar_incidente(reporte, descripcion_incidente, *hora_incidente, *min_incidente, *dia_incidente, *prioridad_incidente);

        incidente_leido = fscanf(archivo_entrada, FORMATO_LECTURA_ARCHIVO, descripcion_incidente, hora_incidente,min_incidente, dia_incidente, prioridad_incidente);
    }

    return archivo_valido;
}

/*
 * PRE: -
 * POST: Devuelve true si 'descripcion', 'hora', 'min', 'dia', 'prioridad' son datos válidos, si algún dato es inválido devuelve false.
 */
bool son_datos_validos(char descripcion[MAX_DESCRIPCION], int hora, int min, char dia, int prioridad){
    if(!es_cant_descripcion_valida(descripcion) || !es_hora_valida(hora, min) || !es_dia_valido(dia) || !es_prioridad_valida(prioridad)){
        return false;
    }

    return true;
}

/*
 * PRE: 'reporte' debe estar previamente creado con 'reporte_crear'.
 * POST: Crea un incidente con los datos ingresados por el usuario, y lo inserta en 'reporte' manteniendo el orden de prioridad.
 */
void ejecutar_comando_agregar_incidente(reporte_t * reporte, char datos[MAX_DATOS], char descripcion[MAX_DESCRIPCION], int hora, int min, char dia, int prioridad ){
    if(!separar_datos(datos, descripcion, &hora, &min, &dia, &prioridad)){
        printf(ROJO_FUERTE "\nEl formato del incidente es inválido.\n\n");
        return; 
    }

    if(!son_datos_validos(descripcion, hora, min, dia, prioridad)){
        printf(ROJO_FUERTE "\nLos datos son inválidos\n\n");
        return;
    }

    insertar_incidente(reporte, descripcion, hora, min, dia, prioridad);
    printf(VERDE_FUERTE "\nEl incidente se agregó correctamente!\n\n" NORMAL BLANCO_FUERTE);
}

/*
 * PRE: 'reporte' debe estar previamente creado con 'reporte_crear'
 * POST: Imprime todos los incidentes dentro de 'reporte' en orden de prioridad.
 */
void ejecutar_comando_imprimir_incidentes(reporte_t * reporte){
    reporte_t* reporte_aux;
    reporte_aux = reporte_crear();
    if(reporte_aux == NULL){
        return;
    }

    incidente_t incidente_eliminado;

    int contador_incidentes = 0;
    while(eliminar_incidente(reporte, &incidente_eliminado)){
        contador_incidentes ++;
        printf("\n\nx ----------------------------------------------------------------------------------- x\n");
        printf(AZUL_FUERTE SUBRAYADO "\nINCIDENTE #%i\n" NORMAL, contador_incidentes);
        mostrar_info_incidente(incidente_eliminado);
        printf("\nx ----------------------------------------------------------------------------------- x\n");
        agregar_incidente(reporte_aux, incidente_eliminado);
        free(incidente_eliminado.descripcion);
    }

    if(contador_incidentes == SIN_INCIDENTES){
        reporte_destruir(reporte_aux);
        printf(ROJO_FUERTE "\nNo hay incidentes, el reporte está vacio\n\n" NORMAL BLANCO_FUERTE);
        return;
    }

    mover_incidentes(reporte_aux, reporte, incidente_eliminado);
    reporte_destruir(reporte_aux);
}

/*
 * PRE: -
 * POST: Verifica que la cantidad de datos sea correcta, y los datos sean válidos.
 *      - Si los datos son inválidos o la cantidad es incorrecta, se pide el reingreso.
 *      - Si los datos son válidos y la cantidad es correcta, crea el incidente.
 */
void verificar_ingreso(incidente_t incidentes[MAX_INCIDENTES], int * tope_incidentes,  int * j, int *cant_datos_leidos, char descripcion[MAX_DESCRIPCION], int hora, int min, char dia, int prioridad){
    while((*cant_datos_leidos) != CANT_DATOS_INCIDENTES){
        printf(ROJO_FUERTE "\nLa cántidad de datos es inválida. Reingrese el incidente %d.\n\n", *j);
        printf( VERDE_FUERTE "%i. " NORMAL BLANCO_FUERTE, *j);
        *cant_datos_leidos = scanf(FORMATO_LECTURA_INGRESO, descripcion, &hora, &min, &dia, &prioridad);
    } 

    while(!son_datos_validos(descripcion, hora, min, dia, prioridad)){
        printf(ROJO_FUERTE "\nLos datos son inválidos. Reingrese el incidente %d.\n\n" NORMAL BLANCO_FUERTE, *j);
        printf( VERDE_FUERTE "%i. " NORMAL BLANCO_FUERTE, *j);
        *cant_datos_leidos = scanf(FORMATO_LECTURA_INGRESO, descripcion, &hora, &min, &dia, &prioridad);
    } 

    crear_incidente(&incidentes[*tope_incidentes], descripcion, hora, min, dia, prioridad);
    (*tope_incidentes) ++;
}

/*
 * PRE: -
 * POST: Reserva memoria en el heap para la descripción de un vector de incidentes.
 */
void reservar_memoria_incidentes(incidente_t incidentes[MAX_INCIDENTES]){
    for(int i = 0; i < MAX_INCIDENTES; i++){
        incidentes[i].descripcion = malloc(sizeof(char) * MAX_DESCRIPCION);
    }
}

/*
 * PRE: 'tope_incidentes' está inicializado.
 *      'incidentes' fue previamente cargado con incidentes válidos.
 * POST: Inserta los incidentes dentro del vector 'incidentes' en 'reporte' respetando el orden.
 */
void insertar_vector_incidentes(reporte_t * reporte, incidente_t incidentes[MAX_INCIDENTES], int tope_incidentes){
    for(int i = 0; i < tope_incidentes; i++){
        int hora = incidentes[i].minutos_desde_medianoche / 60;
        int minutos = incidentes[i].minutos_desde_medianoche % 60;

        insertar_incidente(reporte, incidentes[i].descripcion, hora, minutos, incidentes[i].dia, incidentes[i].prioridad);
    }
}

/*
 * PRE: Se reservó memoria previamente con 'reservar_memoria_incidentes'.
 * POST: Libera la memoria reservada en el heap para la descripcion de un vector de incidentes.
 */
void liberar_memoria_incidentes(incidente_t incidentes[MAX_INCIDENTES]){
    for(int i = 0; i < MAX_INCIDENTES; i++){
        free(incidentes[i].descripcion);
    }
}

/*
 * PRE: 'cant_incidentes' es una cantidad válida.
 * POST: Solicita el ingreso de n incidentes, verificando que los datos sean válidos, y los inserta dentro de 'reporte' respetando 
 * el orden.
 */
void pedir_ingreso_incidentes(reporte_t * reporte, int cant_incidentes, char descripcion[MAX_DESCRIPCION], int hora, int min, char dia, int prioridad){
    incidente_t incidentes[MAX_INCIDENTES];
    reservar_memoria_incidentes(incidentes);

    int tope_incidentes = 0;

    printf("Ingrese:" VERDE_FUERTE " <DESCRIPCIÓN>;<HORA>;<DIA>;<PRIORIDAD>\n\n");

    for(int j = 1; j <= cant_incidentes; j++){
        printf( VERDE_FUERTE "%i. " NORMAL BLANCO_FUERTE, j);
        int cant_datos_leidos = scanf(FORMATO_LECTURA_INGRESO, descripcion, &hora, &min, &dia, &prioridad);
        verificar_ingreso(incidentes, &tope_incidentes, &j, &cant_datos_leidos, descripcion, hora, min, dia, prioridad);
    }

    insertar_vector_incidentes(reporte, incidentes, tope_incidentes);
    liberar_memoria_incidentes(incidentes);
}

/*
 * PRE: -
 * POST: Devuelve true si la cantidad de incidentes a ingresar es mayor a 0, en caso contrario, devuelve false. 
 */
bool es_cantidad_valida(int cantidad){
    return (cantidad > 0);
}

/*
 * PRE: 'reporte' está previamente creado con 'reporte_crear'.
 * POST: Agrega n incidentes dentro de 'reporte', manteniendo el orden de prioridad. 
 */
void ejecutar_comando_agregar_multiples(reporte_t * reporte, char datos[MAX_DATOS], char descripcion[MAX_DESCRIPCION], int hora, int min, char dia, int prioridad){
    int cantidad_incidentes = atoi(datos);
    if(!es_cantidad_valida(cantidad_incidentes)){
        printf(ROJO_FUERTE "\nLa cantidad de datos es inválida\n\n" NORMAL BLANCO_FUERTE);
        return;
    } 

    pedir_ingreso_incidentes(reporte, cantidad_incidentes, descripcion, hora, min, dia, prioridad);
}

/*
 * PRE: 'reporte' y 'reporte_aux' estan previamente creados con 'reporte_crear'
 * POST: Se elimina cada 'incidente' dentro de 'reporte' e incrementa un contador de 'contador_por_dia' según el dia del incidente.
 */
void contar_dias(reporte_t * reporte, reporte_t* reporte_aux, int contador_por_dia[MAX_CONTADORES]){
    incidente_t incidente;

    while(eliminar_incidente(reporte, &incidente)){
        for(int i = 0; i < MAX_CONTADORES; i++){
            switch(i){
                case POS_LUNES: 
                    if(incidente.dia == LUNES_MAYUS){
                        contador_por_dia[i] ++;
                    }
                break;
                case POS_MARTES:
                    if(incidente.dia == MARTES_MAYUS){
                        contador_por_dia[i] ++;
                    }
                break;
                case POS_MIERCOLES:
                    if(incidente.dia == MIERCOLES_MAYUS){
                        contador_por_dia[i] ++;
                    }
                break;
                case POS_JUEVES:
                    if(incidente.dia == JUEVES_MAYUS){
                        contador_por_dia[i] ++;
                    }
                break;
                case POS_VIERNES:
                    if(incidente.dia == VIERNES_MAYUS){
                        contador_por_dia[i] ++;
                    }
                break;
                case POS_SABADO:
                    if(incidente.dia == SABADO_MAYUS){
                        contador_por_dia[i] ++;
                    }
                break;
                case POS_DOMINGO:
                    if(incidente.dia == DOMINGO_MAYUS){
                        contador_por_dia[i] ++;
                    }
                break;
            }
        }

        agregar_incidente(reporte_aux, incidente);
        free(incidente.descripcion);
    }
}

/*
 * PRE: 'contador' debe estar inicializado previamente con 'contar_dias'.
 * POST: Devuelve true si todos los contadores estan vacios, en caso contrario, devuelve false.
 */
bool estan_contadores_vacios(int contador[MAX_CONTADORES]){
    int contadores_vacios = 0;

    for(int i = 0; i < MAX_CONTADORES; i++){
        if(contador[i] == CONTADOR_VACIO){
            contadores_vacios ++;
        }
    }

    if(contadores_vacios == MAX_CONTADORES){
        return true;
    }

    return false;
}

/*
 * PRE: 'contador' debe estar inicializado previamente con 'contar_dias'.
 * POST: Devuelve la posicion del día con mayor cantidad de incidentes.   
 */
int buscar_posicion_dia(int contador[MAX_CONTADORES]){
    int pos_dia_mas_incidentes = 0;

    if(estan_contadores_vacios(contador)){
        pos_dia_mas_incidentes = NO_HAY_DIA_ACCIDENTADO;
    }

    for(int i = 1; i < MAX_CONTADORES; i++){
        if(contador[pos_dia_mas_incidentes] < contador[i]){
            pos_dia_mas_incidentes = i;
        }
    }

    return pos_dia_mas_incidentes;
}

/*
 * PRE: 'reporte' esta previamente creado con 'reporte_crear'.
 * POST: Devuelve la posicion que representa el dia en el que ocurrió la mayor cantidad de incidentes.
 */
int dia_mas_accidentado(reporte_t* reporte){
    reporte_t* reporte_aux;
    reporte_aux = reporte_crear();
    if(reporte_aux == NULL){
        return ERROR;
    }

    incidente_t incidente_eliminado;
    int contadores_dia[MAX_CONTADORES] = {0};

    contar_dias(reporte, reporte_aux, contadores_dia);
    mover_incidentes(reporte_aux, reporte, incidente_eliminado);
    reporte_destruir(reporte_aux);

    int pos_dia_accidentado = buscar_posicion_dia(contadores_dia);

    return pos_dia_accidentado;
}

/*
 * PRE: 'pos_dia' debe estar previamente inicializado con 'buscar_posicion_dia'.
 * POST: Imprime por pantalla un mensaje mostrando el dia más accidentado según la posición del dia recibida por parámetro.
 */
void mostrar_dia_mas_accidentado(int pos_dia){
    if(pos_dia >= POS_LUNES && pos_dia <= POS_DOMINGO){
        printf(VERDE_FUERTE "\nSegún los registros del Sr. Smithers, el dia más accidentado es el día *%s*\n\n" NORMAL BLANCO_FUERTE, NOMBRES_DIAS[pos_dia]);
    } else if(pos_dia == NO_HAY_DIA_ACCIDENTADO){
        printf(VERDE_FUERTE "\nNo hay incidentes registrados, el reporte está vacío\n\n" NORMAL BLANCO_FUERTE);
    }
}

/*
 * PRE: -
 * POST: Imprime por pantalla el dia en el cual ocurrieron más incidentes.
 */
void ejecutar_comando_dia_accidentado(reporte_t * reporte){
    int pos_dia = dia_mas_accidentado(reporte);
    mostrar_dia_mas_accidentado(pos_dia);
}

/*
 * PRE: 'argc' es una cantidad válida de argumentos.
 * POST: Devuelve el archivo de salida según la cantidad de argumentos recibido por línea de comando.
 */
void obtener_archivo_salida(int argc, char * argv[], FILE ** archivo_salida){
    if(argc == MAXIMO_ARGUMENTOS){
        (*archivo_salida) = fopen(argv[2], "w");
    } else {
        (*archivo_salida) = fopen(argv[1], "w");
    }

    if (!(*archivo_salida)) {
        printf(ROJO_FUERTE "\nNo se pudo abrir el archivo de salida\n\n");
        return;
    }
}

/*
 * PRE: 'archivo_salida' esta previamente definido.
 * POST: Inserta todos los incidentes de 'reporte' en 'archivo_salida'
 */
void guardar_reporte(reporte_t * reporte, FILE * archivo_salida){
    incidente_t incidente;

    while(eliminar_incidente(reporte, &incidente)){
        int hora = incidente.minutos_desde_medianoche / 60;
        int minutos = incidente.minutos_desde_medianoche % 60;

        fprintf(archivo_salida, "%s;%02d:%02d;%c;%d\n", incidente.descripcion, hora, minutos, incidente.dia, incidente.prioridad);
        free(incidente.descripcion);
    }

}

/*
 * PRE: 'argc' es una cantidad válida.
 * POST: Guarda el reporte dentro del archivo de salida especificado por línea de comando. Si solo se recibió un archivo, el mismo se sobreescribe con los nuevos cambios.
 */
void ejecutar_comando_salir(int argc, char * argv[], reporte_t * reporte){
    FILE * archivo_salida;
    obtener_archivo_salida(argc, argv, &archivo_salida);
    guardar_reporte(reporte, archivo_salida);
    fclose(archivo_salida);
}

/*
 * PRE: 'comando' es un comando válido.
 *      'reporte' se creó previamente con 'reporte_crear'.
 * POST: Ejecuta el procedimiento correspondiente al comando recibido. 
 */
void ejecutar_comando(char comando[MAX_COMANDO], reporte_t * reporte, char datos[MAX_DATOS], char descripcion[MAX_DESCRIPCION], int hora, int min, char dia, int prioridad, int argc, char * argv[]){
    if(strcmp(comando, COMANDO_ASIGNAR) == 0){
        ejecutar_comando_asignar_incidente(reporte);
    } else if(strcmp(comando, COMANDO_AGREGAR) == 0){
        ejecutar_comando_agregar_incidente(reporte, datos, descripcion, hora, min, dia, prioridad);
    } else if(strcmp(comando, COMANDO_MULTIPLES_INCIDENTES) == 0){
        ejecutar_comando_agregar_multiples(reporte, datos, descripcion, hora, min, dia, prioridad);
    } else if (strcmp(comando, COMANDO_DIA_ACCIDENTADO) == 0){
        ejecutar_comando_dia_accidentado(reporte);
    } else if(strcmp(comando, COMANDO_IMPRIMIR) == 0){
        ejecutar_comando_imprimir_incidentes(reporte);
    } else if(strcmp(comando, COMANDO_SALIR) == 0){
        ejecutar_comando_salir(argc, argv, reporte);
    }
}