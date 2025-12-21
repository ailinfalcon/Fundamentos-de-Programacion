#include "plutonio.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define ARRIBA 'W'
#define ABAJO 'S'
#define DERECHA 'D'
#define IZQUIERDA 'A'
#define LINTERNAS 'L'
#define HOMERO 'H'
#define DONAS 'D'
#define INTERRUPTORES 'I'
#define RATAS_RADIOACTIVAS 'R'
#define BARRILES 'A'
#define BARRAS 'B'
#define ESCALERAS 'E'
#define CHARCOS 'C'
#define COMENZAR_MAYUS 'C'
#define COMENZAR_MINUS 'c'

#define LINTERNA_EMOJI "\U0001f526"
#define ARRIBA_EMOJI "\u2B06\uFE0F"
#define ABAJO_EMOJI "\u2B07\uFE0F"
#define IZQUIERDA_EMOJI "\u2B05\uFE0F"
#define DERECHA_EMOJI "\u27A1\uFE0F"
#define OBJETIVO_EMOJI "\U0001f3af"
#define REGLAS_EMOJI "\U0001f4cc"
#define LUZ_EMOJI "\U0001f4a1"

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

const int JUEGO_GANADO = 1;
const int JUEGO_PERDIDO = -1;

/*
 *  Pre condiciones: -
 *  Post condiciones: Muestra por pantalla un mensaje de bienvenida, con una breve introducción.
 */
void mostrar_mensj_bienvenida(){
    printf("\n\n" BLANCO_FUERTE "¡BIENVENIDO/A A DESASTRE NUCLEAR!" NORMAL "\n\n\n");
    printf(AZUL_FUERTE ">" NORMAL " Después de un largo día en la planta nuclear, Homero solo quería ir rumbo al bar de Moe...\n");
    printf("Pero distraído, tropieza con una caja repleta de barras de plutonio, desparramándolas por todos lados.\n");
    printf(AZUL_FUERTE "Ahora depende de vos ayudarlo a recuperarlas antes de que se quede sin energía y se rinda." NORMAL "\n\n\n\n");
}

/*
 *  Pre condiciones: -
 *  Post condiciones: Imprime los objetivos, reglas y elelemntos del juego.
 */
void mostrar_reglas(){
    printf("%s " BLANCO_FUERTE SUBRAYADO "OBJETIVO" NORMAL ":\n\n", OBJETIVO_EMOJI);
    printf("\033[1;34m>\033[0m Recolectar las 10 barras de plutonio \033[1;34m[B]\033[0m antes de que Homero \033[1;33m[H]\033[0m se quede sin energía\n\n\n");

    printf("%s " BLANCO_FUERTE SUBRAYADO "REGLAS DEL JUEGO" NORMAL ":\n\n", REGLAS_EMOJI);
    printf(AZUL_FUERTE "." NORMAL " Homero empieza con 400 puntos de energía.\n");
    printf(AZUL_FUERTE "." NORMAL " Cada movimiento le consume 1 punto de energía.\n");
    printf(AZUL_FUERTE "." NORMAL " Si la energía llega a 0 antes de juntar las 10 barras de plutonio, perdes!\n");
    printf(AZUL_FUERTE "." NORMAL " El terreno esta oscuro y solo podes ver los elementos si prendes una  %s linterna o te paras sobre un interruptor.\n\n\n", LINTERNA_EMOJI);

    printf("%s " BLANCO_FUERTE SUBRAYADO "ELEMENTOS DEL JUEGO" NORMAL ":\n\n", LUZ_EMOJI);
    printf(AZUL_FUERTE "." NORMAL " DONAS" ROSA_FUERTE " [%c]" NORMAL ": Si agarras una dona, recuperas +10 de energía.\n", DONAS);
    printf(AZUL_FUERTE "." NORMAL " INTERRUPTORES" ROJO_FUERTE " [%c]" NORMAL ": Si te colocas sobre uno, encendes la luz para ver todo.\n", INTERRUPTORES);
    printf(AZUL_FUERTE "." NORMAL " Propulsores: Puede ser muy útiles, pero también muy peligrosos...\n");
    printf(BLANCO_FUERTE "   -" NORMAL " CHARCOS " CELESTE_FUERTE " [%c]" NORMAL "\n", CHARCOS);
    printf(BLANCO_FUERTE "   -" NORMAL " ESCALERAS "CELESTE_FUERTE" [%c]" NORMAL "\n", ESCALERAS);
    printf(AZUL_FUERTE "." NORMAL " LINTERNAS "AZUL_FUERTE " [%c]" NORMAL": Podes ver todos los elementos que se encuentren a tu alrededor, solo permanecerá encendida durante tus próximos 5 movimientos.\n", LINTERNAS);
    printf(AZUL_FUERTE "." NORMAL " RATAS RADIOACTIVAS " BLANCO_FUERTE " [%c]" NORMAL": Ojo! Si chocas alguna, perdes una linterna...\n", RATAS_RADIOACTIVAS);
    printf(AZUL_FUERTE "." NORMAL " BARRILES " VERDE_FUERTE " [%c]" NORMAL ": Si chocas algún barril, perdes -15 de energía.\n\n\n\n\n", BARRILES);
}


/*
 *  Pre condiciones: -
 *  Post condiciones: Solicita al usuario una acción válida para comenzar con el juego.
 */
void pedir_confirmacion(char * confirmacion){
    printf(AZUL_FUERTE ">" NORMAL " ¿Estas listo/a para empezar?\n");
    printf(AZUL_FUERTE ">" NORMAL " Presiona '%c' para comenzar con el juego: ", COMENZAR_MAYUS);
    scanf(" %c", confirmacion);

    while (*confirmacion != COMENZAR_MAYUS && *confirmacion != COMENZAR_MINUS){
        printf("\n" AZUL_FUERTE "> Ingreso inválido\n");
        printf(AZUL_FUERTE ">" NORMAL " Presiona '%c' para comenzar con el juego: ", COMENZAR_MAYUS);
        scanf(" %c", confirmacion);
    }

}

/*
 *  Pre condiciones: -
 *  Post condiciones: Muestra por pantalla una breve introducción sobre que trata el juego.
 */
void mostrar_intro(char * confirmacion){
    mostrar_mensj_bienvenida();
    mostrar_reglas();
    pedir_confirmacion(confirmacion);
}

/*
 *  Pre condiciones: -
 *  Post condiciones: Devuelve true si la accion es válida, en caso contrario devuelve false.
 */
bool es_accion_valida (char accion){
    return (accion == ARRIBA || accion == ABAJO || accion == IZQUIERDA || accion == DERECHA || accion == LINTERNAS);
}

/*
 *  Pre condiciones: -
 *  Post condiciones: Solicita al usuario el ingreso de una acción válida.
 */
void pedir_accion(char * accion){
    printf("\033[1;34m>\033[0m Por favor, ingrese su jugada: ");
    scanf(" %c", accion);

    while(!es_accion_valida(*accion)){
        printf("\n");
        printf(AZUL_FUERTE ">" NORMAL " Ingresó un movimiento inválido!\n");
        printf(AZUL_FUERTE "> Ingrese un movimiento válido: \033[0m");
        scanf(" %c", accion);
    }
}

/*
 *  Pre condiciones: -
 *  Post condiciones: Imprime por pantalla un dibujo de homero 
 */
void mostrar_homero_ganador(){
    printf( AMARILLO_FUERTE "\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
    printf("\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⡠⠴⢿⣍⣩⡿⠦⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
    printf("\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡠⠞⠋⠀⠀⠀⠀⠀⠈⠙⡦⣵⠙⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
    printf("\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠊⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠿⢷⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ---------------------------------------------------------");
    printf("\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣴⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀| ¡WOHOOO! Homero logró juntar las 10 barras de plutonio  |");
    printf("\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣞⣥⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀|                        ¡GANASTE!                        |");
    printf("\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡾⠃⠀⠀⠀⠙⣦⠖⠋⠉⠉⠳⢦⠀⠀⠀⠀⠀⢸⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⠴⠺⢻⣆⠀⠀⠀ /---------------------------------------------------------");
    printf("\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⡇⠀⡷⠀⠀⢰⠃⠀⠀⠀⠀⠀⠀⣷⠀⠀⠀⢠⠏⠀⠀⠀⠀⠀⢀⣠⠤⠤⢤⠜⠃⠀⢠⡼⠋⠀⠀ /");
    printf("\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⡷⠒⠒⠦⠤⢼⡀⠀⠀⠀⠰⠒⢀⡿⢀⣤⣰⠟⠀⠀⠀⠀⠀⣴⣫⠀⠀⠀⠀⠀⠀⠀⣿⡄⠀⠀⠀⠀⠀");
    printf("\n⠀⠀⠀⠀⠀⠀⠀⢀⣤⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⡷⢷⣀⠀⠀⠀⠀⠉⠉⠓⠲⣤⣤⠞⣠⡞⣿⣏⣤⡷⠀⠀⢰⣞⡑⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⠀⠀⠀⠀⠀");
    printf("\n⠀⠀⠀⠀⠀⠀⢰⠎⠀⠰⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⠟⠁⠀⠀⠉⠉⠛⠒⠒⠚⠛⠒⠶⢬⡀⠀⢽⣾⣿⣏⣰⠇⠀⢀⡾⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⢸⢿⠀⠀⠀⠀⠀");
    printf("\n⠀⠀⠀⠀⠀⢀⣼⠈⠀⢰⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢷⣀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣤⣶⠶⠶⣿⣧⣨⣿⣿⡿⠋⠀⠀⢸⢠⣠⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢧⠀⠀⠀⠀");
    printf("\n⠀⣴⠞⣻⣯⣏⠀⡀⠀⠀⢻⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠾⠤⢤⡤⢤⡴⢿⣿⣋⣵⣷⣴⣿⣿⣿⣿⢹⡁⠀⠀⠀⠀⠈⠳⠼⠷⢤⣀⡀⠀⠀⠀⠀⠀⠀⠀⠁⢳⡀⠀⠀");
    printf("\n⠸⣷⠀⢁⠀⣹⡇⢷⡀⠀⠀⢧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠋⠉⠉⠻⣿⣿⣿⣿⣿⣿⣿⣿⢹⠀⣷⣦⡀⠀⠀⠀⠀⠀⠀⠀⠈⠻⡄⠀⠀⠀⠀⠀⠀⠀⠀⢱⡀⠀");
    printf("\n⠀⣿⠛⠚⠋⠁⣿⠀⠙⠂⠀⢈⢣⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⣿⣾⣿⣿⣿⣿⣿⣏⡿⠀⣸⠏⢳⡀⠀⠀⠀⠀⠀⠀⠀⠀⢹⡄⠀⠀⠀⠀⠀⠀⠀⠀⢳⠀");
    printf("\n⠀⢹⡷⠶⠖⠛⢻⡄⠀⠀⠀⠀⠀⠙⢦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⣿⣿⣏⠛⠿⣿⣛⣻⣿⣿⣁⡼⠋⠀⠀⣳⡤⠖⠒⠋⢉⣿⣛⣦⠤⣷⠀⠀⠀⠀⠀⠀⠀⠀⠘⡄");
    printf("\n⠀⠈⠳⠦⠤⠴⡟⠀⠀⠀⠀⠀⠀⠀⠁⢝⠦⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⢿⠛⢿⣷⣀⣰⣿⣿⡿⠟⠉⠀⠀⢠⠞⠁⠀⠀⠀⢀⡾⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇");
    printf("\n⠀⠀⠀⠀⠀⠀⠻⡆⠀⠀⠀⠀⠀⠀⠀⠀⠈⠪⢝⡲⠤⣀⣠⣖⠛⠉⠉⠉⡟⠀⣳⣼⠀⠀⢨⠟⠹⡄⠀⠀⠀⢠⠏⠀⠀⠀⠀⠀⢸⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⠃");
    printf("\n⠀⠀⠀⠀⠀⠀⠀⠙⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠀⠀⠉⠉⣇⠀⠀⢰⣇⡼⠁⠙⣦⣰⠏⠀⠀⢻⡀⢀⣠⣿⠀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡞⠀");
    printf("\n⠀⠀⠀⠀⠀⠀⠀⠀⠈⠳⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⢀⡟⠁⠀⠀⠈⠁⠀⠀⠀⠘⠗⠋⠀⢻⡀⠀⠀⠀⠀⠀⢸⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠏⠀⠀");
    printf("\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠳⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⠀⠀⡼⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢳⡁⠀⠀⠀⠀⠘⣇⠀⠀⠀⠀⠀⠀⠀⢀⣀⡴⠞⠁⠀⠀⠀");
    printf("\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠦⣄⡀⠀⠀⠀⠀⠀⠀⠀⡞⠀⢸⠇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠳⣄⡀⠀⠀⠘⣦⡤⣤⡤⠤⠖⠚⠋⠀⠀⠀⠀⠀⠀⠀");
    printf("\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠓⠦⢤⣄⣀⣀⣴⠃⠀⡞⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⠉⠙⠒⠲⡚⠛⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
    printf("\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠙⠓⠲⢶⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢆⠀⠀⠀⠹⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
    printf("\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⡾⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⡆⠀⠀⠀⢹⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
    printf("\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡴⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⠀⠀⠀⠀⢷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
    printf("\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡾⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⡇⠀⠀⠀⠈⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
    printf("\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡞⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠁⠀⠀⠀⠀⣧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
    printf("\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⠿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀" NORMAL "\n\n\n\n");
}

/*
 *  Pre condiciones: -
 *  Post condiciones: Imprime por pantalla un dibujo de homero.
 */
void mostrar_homero_perdedor(){
    printf(AMARILLO_FUERTE "\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⡀⠤⠤⠤⢀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
    printf("\n⠀⠀⠀⢀⠔⠉⢁⡨⠛⠉⠀⠀⠀⠀⠀⠀⠀⠉⠒⢤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
    printf("\n⠀⠀⢠⡟⠊⡱⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠒⠤⠤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
    printf("\n⠀⢰⠃⢣⠞⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠁⠲⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ -------------------------------------------------------");
    printf("\n⠀⢧⡀⡞⠓⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠐⠢⣄⠀⣀⠤⠶⠚⢹⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀| Homero se quedó sin energía y se fue por una cerveza  |");
    printf("\n⠀⠀⢹⡅⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡠⠔⠚⢻⠀⢀⠔⠉⠉⢣⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀|                    ¡PERDISTE!                         |");
    printf("\n⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡄⠀⠀⠀⣠⡾⠅⠀⠀⠀⢘⡶⠋⠀⠀⠀⢸⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀/ ------------------------------------------------------" );
    printf("\n⠀⠀⠘⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⡀⢀⠞⠁⠀⠀⠀⠀⠀⡜⠀⠀⠀⣀⠴⠊⠉⠉⠁⠒⠤⢄⡀⠀⠀⠀/⠀");
    printf("\n⠀⠀⠀⠱⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠱⣏⠀⠀⠀⠀⠀⡠⠞⠀⢀⡴⠊⠁⠀⠀⠀⠀⠀⠀⠀⢀⠙⣦⠀⠀⠀");
    printf("\n⠀⠀⠀⠀⠈⢦⡀⠀⢶⣄⡀⠀⠀⠀⠀⠀⠉⠐⠒⠂⠉⠀⢀⡔⠋⠀⠀⠀⠀⠀⠀⣀⠤⠒⠊⣹⣧⡟⠀⠀⠀");
    printf("\n⠀⠀⠀⠀⠀⠀⢨⣦⡀⠳⡉⠲⢄⡀⠀⠀⠀⠀⠀⠀⠀⡰⠋⠀⠀⠀⠀⠀⣠⠔⠋⢀⣰⢺⣿⣻⠏⠀⠀⠀⠀");
    printf("\n⠀⠀⠀⠀⠀⠀⠀⠩⡉⠳⢿⣄⣀⡀⠀⠀⠀⠀⠀⠀⡼⠁⠀⠀⠀⠀⡠⠊⢁⣼⢟⣩⠞⡿⠊⠁⠀⠀⠀⠀⠀");
    printf("\n⠀⠀⠀⠀⠀⠀⠀⠀⠙⠂⢰⠋⣠⡤⠀⠀⠀⠀⠀⡼⠁⠀⠀⠀⣠⠎⠀⣾⡟⢡⠞⡠⠊⠀⠀⠀⠀⠀⠀⠀⠀");
    printf("\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢦⣉⣃⣀⡾⠀⠀⢰⠁⠀⠀⠀⡴⢡⣴⣾⣿⣿⢋⡞⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀");
    printf("\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⡇⠀⠀⠀⡞⠀⠀⠀⣼⣥⣾⣿⣿⣿⣯⢸⡁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
    printf("\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇⠀⠀⠀⣇⠀⠀⢸⣿⣿⣿⣿⡟⠿⣿⠆⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀");
    printf("\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡇⠀⠀⠀⠸⡄⠀⢸⣷⣿⣾⣧⢳⣶⣿⡇⢸⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀");
    printf("\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡇⠀⠀⠀⠀⠙⢄⠀⠉⢻⠀⠹⣿⣿⠿⡷⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
    printf("\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⠛⢧⣀⠀⠀⠀⠀⠀⠙⠒⠼⣄⣀⠀⠀⠀⣷⢤⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
    printf("\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⠃⠀⠀⠈⠉⠒⠦⠤⣀⡀⠀⠀⠀⠈⠙⣆⠘⢹⡾⠉⢢⠀⠀⠀⠀⠀⠀⠀⠀");
    printf("\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⢹⢦⡀⠀⠀⠀⡼⢧⠀⠈⡇⠀⠀⠀⠀⠀⠀⠀");
    printf("\n⠀⠀⠀⠀⠀⣀⠤⠄⠒⠒⠒⠛⠿⣦⣄⡀⠀⠀⠀⠀⠀⠀⠀⡜⠀⠑⣄⠀⠀⡇⠈⢆⠀⣹⠤⣀⠀⠀⠀⠀⠀");
    printf("\n⠀⠀⢀⠔⠋⠀⠀⠀⠀⠀⠀⠀⠀⠈⠓⡌⠑⠢⢄⣀⠀⠀⢠⠁⠀⠀⠈⢣⣠⠃⠀⠈⢶⣧⠀⠀⠉⠒⢄⠀⠀");
    printf("\n⠀⡰⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⡀⠀⠀⠀⠉⠑⠋⠀⠀⠀⠀⠀⠹⠀⠀⠀⠀⠈⢣⠀⠀⠀⠀⠙⠆");
    printf("\n⠰⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⠧⠀⠀⠀⠀⠀" NORMAL "\n\n\n\n");
}


/*
 *  Pre condiciones: -
 *  Post condiciones: Muestra por pantalla un mensaje al final del juego, dependiendo si esta ganado o perdido.
 */
void mostrar_resultado_jugada(juego_t juego){
    if(estado_juego(juego) == JUEGO_GANADO){
        mostrar_homero_ganador();
    } else if(estado_juego(juego) == JUEGO_PERDIDO){
        mostrar_homero_perdedor();
    }
}

int main(){
    juego_t juego;
    char confirmacion = ' ';
    char accion = ' ';

    srand((unsigned)time(NULL));

    mostrar_intro(&confirmacion);
    system("clear");
    inicializar_juego(&juego);
    mostrar_juego(juego);
    printf("\n");

    while(estado_juego(juego) == 0){
        pedir_accion(&accion);
        realizar_jugada(&juego, accion);
        system("clear");
        mostrar_juego(juego);
        printf("\n");
    }

    system("clear");
    printf("\n");
    mostrar_resultado_jugada(juego);

    return 0;
}