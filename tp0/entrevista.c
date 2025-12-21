#include <stdio.h>
#include <stdbool.h>

const int MINIMA_EXPERIENCIA = 0;
const int MAXIMA_EXPERIENCIA = 300;
const int MINIMO_DONAS = 0;
const int MAXIMO_DONAS = 12;

const char PROTOCOLO_DE_SEGURIDAD = 'P';
const char CORRER = 'C';
const char PILETA = 'S';
const char DORMIR = 'D';
const char TRABAJO_MATERIALES_RADIACTIVOS = 'S';
const char NO_TRABAJO_MATERIALES_RADIACTIVOS = 'N';

#define NORMAL "\e[0m"
#define SUBRAYADO "\e[4m"
#define AMARILLO_FUERTE "\e[33;1m"
#define ROJO_FUERTE "\e[31;1m"
#define AZUL_FUERTE "\e[34;1m"
#define AZUL "\e[34m"
#define ROSA_FUERTE "\e[35;1m"
#define CELESTE_FUERTE "\e[36;1m"
#define BLANCO_FUERTE "\e[1m"
#define VERDE_FUERTE "\e[32;1m"


/*
    PRE: -
    POST: Devuelve true si la cantidad de meses ingresados es válida, en caso contrario devuelve false.
*/
bool es_cant_meses_valida(float meses){
    return ((meses >= MINIMA_EXPERIENCIA) && (meses <= MAXIMA_EXPERIENCIA));
}

/*
    PRE: El número ingresado debe ser mayor o igual a 0 y menor o igual a 300.
    POST: Guarda el valor ingresado sobre la experiencia en el sector nuclear en la variable 'experiencia_sector_nuclear'.
*/
void preguntar_experiencia_sector_nuclear (float* experiencia_sector_nuclear){
    printf(AZUL_FUERTE ">" NORMAL " ¿Cuántos meses (completos) tiene de experiencia en el sector nuclear? La respuesta debe estar entre %i y %i (inclusive)\n", MINIMA_EXPERIENCIA, MAXIMA_EXPERIENCIA);
    scanf("%f", experiencia_sector_nuclear);

    while(!es_cant_meses_valida(*experiencia_sector_nuclear)){
        printf(ROJO_FUERTE "\nPor favor, ingrese una respuesta válida.\n");
        printf(AZUL_FUERTE ">" NORMAL " ¿Cuántos meses (completos) tiene de experiencia en el sector nuclear? La respuesta debe estar entre %i y %i (inclusive)\n", MINIMA_EXPERIENCIA, MAXIMA_EXPERIENCIA);
        scanf("%f", experiencia_sector_nuclear);
    }
}

/*
    PRE: -
    POST: Devuelve true si la opcion es válida, en caso contrario devuelve false.
*/
bool es_opcion_valida(char opcion){
    return ((opcion == PROTOCOLO_DE_SEGURIDAD) || (opcion == CORRER) || (opcion == PILETA) || (opcion == DORMIR));
}

/*
    PRE: -
    POST: Guarda una opción valida ingresada sobre el caso de emergencia en 'opcion_caso_de_emergencia'.
*/
void preguntar_caso_de_emergencia (char* opcion_caso_de_emergencia){
    printf(AZUL_FUERTE "\n>" NORMAL " En caso de emergencia, ¿cómo actuaría primero?\n\n");
    printf("- Activar el protocolo de seguridad" VERDE_FUERTE " [%c]\n", PROTOCOLO_DE_SEGURIDAD);
    printf(NORMAL "- Correr" AMARILLO_FUERTE " [%c]\n", CORRER);
    printf(NORMAL "- Saltar del 9.º a la pileta" ROJO_FUERTE " [%c]\n", PILETA);
    printf(NORMAL "- Seguir durmiendo" ROSA_FUERTE " [%c]\n" NORMAL, DORMIR);
    scanf(" %c", opcion_caso_de_emergencia);

    while (!es_opcion_valida(*opcion_caso_de_emergencia)){
        printf(ROJO_FUERTE "\nPor favor, ingrese una opción válida (%c, %c, %c o %c): \n", PROTOCOLO_DE_SEGURIDAD, CORRER, PILETA, DORMIR);
        printf(AZUL_FUERTE "\n>" NORMAL " En caso de emergencia, ¿cómo actuaría primero?\n\n");
        printf("- Activar el protocolo de seguridad" VERDE_FUERTE " [%c]\n", PROTOCOLO_DE_SEGURIDAD);
        printf(NORMAL "- Correr" AMARILLO_FUERTE " [%c]\n", CORRER);
        printf(NORMAL "- Saltar del 9.º a la pileta" ROJO_FUERTE " [%c]\n", PILETA);
        printf(NORMAL "- Seguir durmiendo" ROSA_FUERTE " [%c]\n" NORMAL, DORMIR);
        scanf(" %c", opcion_caso_de_emergencia);
    }

}

/*
    PRE: -
    POST: Devuelve true si la respuesta es válida, en caso contrario devuelve false.
*/
bool es_respuesta_valida(char respuesta){
    return((respuesta == TRABAJO_MATERIALES_RADIACTIVOS) || (respuesta == NO_TRABAJO_MATERIALES_RADIACTIVOS));
}

/*
    PRE: -
    POST: Guarda true o false según una opción valida ingresada sobre si trabajo o no con materiales radiactivos en 'trabajo_materiales_radiactivos'.
*/
void preguntar_materiales_radiactivos(char* si_o_no_materiales_radiactivos, bool* trabajo_materiales_radiactivos){
    printf(AZUL_FUERTE "\n>" NORMAL " ¿Ha trabajado previamente con materiales radiactivos? [S/N]\n");
    scanf(" %c", si_o_no_materiales_radiactivos);

    while(!es_respuesta_valida(*si_o_no_materiales_radiactivos)){
        printf(ROJO_FUERTE "\nPor favor, ingrese una opción válida (%c o %c): \n", TRABAJO_MATERIALES_RADIACTIVOS, NO_TRABAJO_MATERIALES_RADIACTIVOS );
        printf(AZUL_FUERTE "\n>" NORMAL " ¿Ha trabajado previamente con materiales radiactivos? [%c/%c]\n", TRABAJO_MATERIALES_RADIACTIVOS, NO_TRABAJO_MATERIALES_RADIACTIVOS);
        scanf(" %c", si_o_no_materiales_radiactivos);
    }

    if(*si_o_no_materiales_radiactivos == TRABAJO_MATERIALES_RADIACTIVOS){
        *trabajo_materiales_radiactivos = true;
    } else if (*si_o_no_materiales_radiactivos == NO_TRABAJO_MATERIALES_RADIACTIVOS){
        *trabajo_materiales_radiactivos = false;
    }

}

/*
    PRE: -
    POST: Devuelve true si la cantidad de donas es válida, en caso contrario devuelve false.
*/
bool es_cant_donas_valida(float cant_donas){
    return((cant_donas >= MINIMO_DONAS) && (cant_donas <= MAXIMO_DONAS));
}

/*
    PRE: La cantidad de donas ingresadas debe ser mayor o igual a 0 y menor o igual a 12.
    POST: Guarda una cantidad valida ingresada de donas en 'cantidad_donas'.
*/
void preguntar_cantidad_donas (float* cantidad_donas){
    printf(AZUL_FUERTE "\n>" NORMAL " ¿Cuántas donas podrías comer en una jornada laboral? La respuesta debe ser un número entre %i y %i (inclusive)\n", MINIMO_DONAS, MAXIMO_DONAS);
    scanf("%f", cantidad_donas);

    while(!es_cant_donas_valida(*cantidad_donas)){
        printf(ROJO_FUERTE"\nPor favor, ingrese una respuesta válida\n");
        printf(AZUL_FUERTE "\n>" NORMAL " ¿Cuántas donas podrías comer en una jornada laboral? La respuesta debe ser un número entre %i y %i (inclusive)\n", MINIMO_DONAS, MAXIMO_DONAS);
        scanf("%f", cantidad_donas);
    }
}

/*
    PRE: EL valor de la variable 'experiencia_sector_nuclear' debe ser mayor o igual a 0 y menor o igual a 300.
    POST: Devuelve la cantidad de meses de experiencia convertida a años.
*/
int calculo_anios_de_experiencia(float experiencia_sector_nuclear){
    int anios_de_experiencia = (int)(experiencia_sector_nuclear/12);
    return anios_de_experiencia;
}

/*
    PRE: -
    POST: Devuelve la cantidad de puntos obtenidos dependiendo de la opción ingresada en caso de emergencia.
*/
int calculo_puntos_caso_de_emergencia(char opcion_caso_de_emergencia){
    int caso_de_emergencia = 0;

    if(opcion_caso_de_emergencia == PROTOCOLO_DE_SEGURIDAD){
        caso_de_emergencia += 15;
    } else if (opcion_caso_de_emergencia == CORRER){
        caso_de_emergencia += 10;
    } else if (opcion_caso_de_emergencia == PILETA){
        caso_de_emergencia += 5;
    }

    return caso_de_emergencia;
}


/*
    PRE: -
    POST: Devuelve la cantidad de puntos obtenidos dependiendo si trabajo o no con materiales radiactivos.
*/
int calculo_puntos_exp_con_materiales (bool trabajo_materiales_radiactivos){
    int experiencia_con_materiales = 0;

    if (trabajo_materiales_radiactivos) { 
        experiencia_con_materiales += 10;
    }

    return experiencia_con_materiales;
}

/*
    PRE: -
    POST: Devuelve el puntaje total de acuerdo con los puntos obtenidos por las respuestas ingresadas.
*/

int calculo_puntaje_total (float experiencia_sector_nuclear, char opcion_caso_de_emergencia, bool trabajo_materiales_radiactivos, float cantidad_donas ){

    int puntaje_total = 0;
    float anios_de_experiencia = (float)calculo_anios_de_experiencia(experiencia_sector_nuclear);
    float puntos_caso_emergencia = (float)calculo_puntos_caso_de_emergencia(opcion_caso_de_emergencia);
    float puntos_exp_con_materiales = (float)calculo_puntos_exp_con_materiales(trabajo_materiales_radiactivos);

    puntaje_total = (int)(anios_de_experiencia + puntos_caso_emergencia + puntos_exp_con_materiales - cantidad_donas);


    return puntaje_total;
}

/*
    PRE: -
    POST: Muestra por pantalla el puesto de trabajo asignado dependiendo del puntaje total obtenido.
*/
void mostrar_puesto_de_trabajo(float experiencia_sector_nuclear, char opcion_caso_de_emergencia, bool trabajo_materiales_radiactivos, float cantidad_donas){
    int puntaje_total = calculo_puntaje_total(experiencia_sector_nuclear, opcion_caso_de_emergencia, trabajo_materiales_radiactivos, cantidad_donas);

    if(puntaje_total <= 15){
        printf(VERDE_FUERTE "\nCon las respuestas brindadas, tu posición es: -MANTENIMIENTO-\n");
    } else if(puntaje_total >= 16 && puntaje_total <= 25){
        printf(VERDE_FUERTE "\nCon las respuestas brindadas, tu posición es: -OPERATIVO-\n");
    } else if(puntaje_total >= 26 && puntaje_total <= 35){
        printf(VERDE_FUERTE "\nCon las respuestas brindadas, tu posición es: -TECNICO-\n");
    } else if(puntaje_total >= 36){
        printf(VERDE_FUERTE "\nCon las respuestas brindadas, tu posición es: -INGENIERO-\n");
    }
}

int main(){

    float experiencia_sector_nuclear = 0;
    preguntar_experiencia_sector_nuclear(&experiencia_sector_nuclear);

    char opcion_caso_de_emergencia = ' ';
    preguntar_caso_de_emergencia(&opcion_caso_de_emergencia);

    char si_o_no_materiales_radioactivos =  ' ';
    bool trabajo_materiales_radiactivos = false;
    preguntar_materiales_radiactivos(&si_o_no_materiales_radioactivos, &trabajo_materiales_radiactivos);

    float cantidad_donas = 0;
    preguntar_cantidad_donas(&cantidad_donas);

    mostrar_puesto_de_trabajo(experiencia_sector_nuclear, opcion_caso_de_emergencia, trabajo_materiales_radiactivos, cantidad_donas);

    return 0;
}