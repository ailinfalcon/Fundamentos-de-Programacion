#include "plutonio.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MAX_FILAS 20
#define MAX_COLUMNAS 20
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

#define LINTERNA_EMOJI "\U0001f526"
#define ARRIBA_EMOJI "\u2B06\uFE0F"
#define ABAJO_EMOJI "\u2B07\uFE0F"
#define IZQUIERDA_EMOJI "\u2B05\uFE0F"
#define DERECHA_EMOJI "\u27A1\uFE0F"

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

const int CANT_LINTERNAS_INICIAL = 5;
const int CANT_BARRAS_INICIAL = 0;
const int ENERGIA_INICIAL = 400;
const int MOV_LINTERNA_INICIAL = 0;
const int CANT_DONAS = 5;
const int CANT_INTERRUPTORES = 4;
const int CANT_RATAS_RADIOACTIVAS = 5;
const int CANT_BARRILES = 15;
const int CANT_BARRAS = 10;
const int CANT_ESCALERAS = 3;
const int CANT_CHARCOS = 3;
const int MAX_DISTANCIA_ELEMENTOS = 2;
const int ENERGIA_DONA = 10;
const int PUNTOS_CHOCA_BARRIL = 15;
const int LINTERNA_RATAS = 1;
const int MAX_MOVIMIENTOS_ESCALERA = 3;
const int MAX_MOVIMIENTOS_CHARCO = 3;
const int MOVIMIENTOS_CON_LINTERNA = 5;
const int GANADO = 1;
const int PERDIDO = -1;
const int SIGUE_JUGANDO = 0;
const int SIN_ENERGIA = 0;


/*
 *  Pre condiciones: -
 *  Post condiciones: Devuelve true si la posición esta vacía, de lo contrario devuelve false.
 */
bool posicion_libre (coordenada_t posicion_aleatoria, juego_t juego){
    bool esta_libre = true;

    if(posicion_aleatoria.fil == juego.homero.posicion.fil && posicion_aleatoria.col == juego.homero.posicion.col){
        esta_libre = false;
    }

    for(int i = 0; i < juego.cantidad_herramientas; i++){
        if(posicion_aleatoria.fil == juego.herramientas[i].posicion.fil && posicion_aleatoria.col == juego.herramientas[i].posicion.col){
            esta_libre = false;
        }
    }

    for(int i = 0; i < juego.cantidad_obstaculos; i++){
        if(posicion_aleatoria.fil == juego.obstaculos[i].posicion.fil && posicion_aleatoria.col == juego.obstaculos[i].posicion.col){
            esta_libre = false;
        }
    }

    for(int i = 0; i < juego.cantidad_barras; i++){
        if(posicion_aleatoria.fil == juego.barras[i].posicion.fil && posicion_aleatoria.col == juego.barras[i].posicion.col){
            esta_libre = false;
        }
    }
    
    for(int i = 0; i < juego.cantidad_propulsores; i++){
        if(posicion_aleatoria.fil == juego.propulsores[i].posicion.fil && posicion_aleatoria.col == juego.propulsores[i].posicion.col){
            esta_libre = false;
        }
    }

    return esta_libre;
}

/*
 *  Pre condiciones: -
 *  Post condiciones: Devuelve una posición aleatoria libre dentro de los rangos del terreno.
 */
coordenada_t posicion_aleatoria(juego_t juego){
    coordenada_t posicion;

    do {
        posicion.fil = rand() % MAX_FILAS;
        posicion.col = rand() % MAX_COLUMNAS;
    } while(!posicion_libre(posicion, juego));

    return posicion;
}

/*
 *  Pre condiciones: -
 *  Post condiciones: Inicializa a homero dentro del juego, se le asigna una posición inicial aleatoria, carga la cantidad de linternas y movimientos iniciales,
 *  la cantidad de barras, la energia inicial y si la linterna esta encendida o no.
 */
void inicializar_homero(juego_t * juego){
    juego->homero.posicion = posicion_aleatoria(*juego);
    juego->homero.cantidad_linternas = CANT_LINTERNAS_INICIAL;
    juego->homero.cantidad_barras = CANT_BARRAS_INICIAL;
    juego->homero.energia = ENERGIA_INICIAL;
    juego->homero.linterna_activada = false;
    juego->homero.mov_linterna = MOV_LINTERNA_INICIAL;
}

/*
 *  Pre condiciones: El tope 'cantidad_herramientas' debe estar inicializado correctamente.
 *  Post condiciones: Inicializa las herramientas dentro del juego, a cada herramienta se le asigna una posición inicial aleatoria, 
 *  el tipo de herramienta y si esta visible o no.
 */
void inicializar_herramientas(juego_t * juego, int cant_donas, int cant_interruptores){
    juego->cantidad_herramientas = cant_donas + cant_interruptores;

    for(int i = 0; i < cant_donas; i++){
        juego->herramientas[i].posicion = posicion_aleatoria(*juego);
        juego->herramientas[i].tipo = DONAS;
        juego->herramientas[i].visible = false;
    }

    for(int i = cant_donas; i < juego->cantidad_herramientas; i++){
        juego->herramientas[i].posicion = posicion_aleatoria(*juego);
        juego->herramientas[i].tipo = INTERRUPTORES;
        juego->herramientas[i].visible = true;
    }
}

/*
 *  Pre condiciones: El tope 'cantidad_obstaculos' debe estar inicializado correctamente.
 *  Post condiciones: Inicializa los obstáculos dentro del juego,  a cada obstáculo se le asigna una posición inicial aleatoria, 
 *  el tipo y si esta visible o no.
 */
void inicializar_obstaculos(juego_t * juego, int cant_ratas, int cant_barriles){
    juego->cantidad_obstaculos = cant_ratas + cant_barriles;

    for(int i = 0; i < cant_ratas; i++){
        juego->obstaculos[i].posicion = posicion_aleatoria(*juego);
        juego->obstaculos[i].tipo = RATAS_RADIOACTIVAS;
        juego->obstaculos[i].visible = false;
    }

    for(int i = cant_ratas; i < juego->cantidad_obstaculos; i++){
        juego->obstaculos[i].posicion = posicion_aleatoria(*juego);
        juego->obstaculos[i].tipo = BARRILES;
        juego->obstaculos[i].visible = false;
    }
}

/*
 *  Pre condiciones: El tope 'cantidad_barras' debe estar inicializado correctamente.
 *  Post condiciones: Inicializa las barras de plutonio dentro del juego, a cada barra de plutonio se le asigna una posición inicial aleatoria 
 *  y si esta visible o no.
 */
void inicializar_barras(juego_t * juego, int cant_barras){
    juego->cantidad_barras = cant_barras;
    for(int i = 0; i < cant_barras; i++){
        juego->barras[i].posicion = posicion_aleatoria(*juego);
        juego->barras[i].tipo = BARRAS;
        juego->barras[i].visible = false;
    }
}

/*
 *  Pre condiciones: El tope 'cantidad_propulsores' debe estar inicializado correctamente.
 *  Post condiciones: Inicializa los propulsores dentro del juego, a cada propulsor se le asigna una posición inicial aleatoria, 
 *  el tipo y si esta visible o no.
 */
void inicializar_propulsores(juego_t * juego, int cant_escaleras, int cant_charcos){
    juego->cantidad_propulsores = cant_escaleras + cant_charcos;
    for(int i = 0; i < cant_escaleras; i++){
        juego->propulsores[i].posicion = posicion_aleatoria(*juego);
        juego->propulsores[i].tipo = ESCALERAS;
        juego->propulsores[i].visible = false;
    }

    for(int i = cant_escaleras; i < juego->cantidad_propulsores; i++){
        juego->propulsores[i].posicion = posicion_aleatoria(*juego);
        juego->propulsores[i].tipo = CHARCOS;
        juego->propulsores[i].visible = false;
    }
}  

/*
 *  Pre condiciones: -
 *  Post condiciones: Inicializará el juego, cargando toda la información inicial de Homero, las herramientas y los obstáculos.
 */
void inicializar_juego(juego_t *juego){
    juego->cantidad_barras = 0;
    juego->cantidad_herramientas = 0;
    juego->cantidad_obstaculos = 0;
    juego->cantidad_propulsores = 0;

    inicializar_homero(juego);
    inicializar_herramientas(juego, CANT_DONAS, CANT_INTERRUPTORES);
    inicializar_obstaculos(juego, CANT_RATAS_RADIOACTIVAS, CANT_BARRILES);
    inicializar_barras(juego, CANT_BARRAS);
    inicializar_propulsores(juego, CANT_ESCALERAS, CANT_CHARCOS);
}

/*
 *  Pre condiciones: -
 *  Post condiciones: Devuelve true si la posición esta dentro de los limites del terreno, en caso contrario, devuelve false.
 */
bool es_movimiento_valido(coordenada_t posicion){
    return ((posicion.fil >= 0 && posicion.fil < MAX_FILAS) && (posicion.col >= 0 && posicion.col < MAX_COLUMNAS));
}

/*
 *  Pre condiciones: El juego debe estar inicializado previamente con `inicializar_juego`.
 *  Post condiciones: Le asigna una nueva posición aleatoria a las ratas radioactivas dentro del juego.
 */
void mover_ratas(juego_t * juego){
    for(int i = 0; i < juego->cantidad_obstaculos; i++){
        if(juego->obstaculos[i].tipo == RATAS_RADIOACTIVAS){
            juego->obstaculos[i].posicion = posicion_aleatoria(*juego);
        }
    }
}

/*
 *  Pre condiciones: El char 'accion' debe estar previamente válidado.
 *  Post condiciones: De acuerdo a la accion:
 *  --> Mueve a homero dentro del juego, verificando que este dentro de los limites del terreno, y restandole la energía correspondiente, 
 *      y en caso que la linterna este encendida, se le suma 1 a 'mov_linterna' de 'homero'.
 *  --> O enciende la linterna, restandole 1 a 'cantidad_linternas' de 'homero', sumandole 1 a 'mov_linterna' y 
 *      hace una llamada a la función mover_ratas.
 */
void realizar_movimiento(juego_t * juego, char accion){
    coordenada_t nueva_posicion = juego->homero.posicion;

    switch (accion){
        case ARRIBA: 
            nueva_posicion.fil -= 1;
            if(es_movimiento_valido(nueva_posicion)){
                juego->homero.posicion = nueva_posicion;
                juego->homero.energia -= 1;

                if(juego->homero.linterna_activada == true){
                    juego->homero.mov_linterna ++;
                }
            }
            break;
        case ABAJO: 
            nueva_posicion.fil += 1;
                if(es_movimiento_valido(nueva_posicion)){
                    juego->homero.posicion = nueva_posicion;
                    juego->homero.energia -= 1;

                    if(juego->homero.linterna_activada == true){
                        juego->homero.mov_linterna ++;
                    }
                }
        break;
        case DERECHA: 
            nueva_posicion.col += 1;
            if(es_movimiento_valido(nueva_posicion)){
                juego->homero.posicion = nueva_posicion;
                juego->homero.energia -= 1;

                if(juego->homero.linterna_activada == true){
                    juego->homero.mov_linterna ++;
                }
            }
        break;
        case IZQUIERDA:
            nueva_posicion.col -= 1;
            if(es_movimiento_valido(nueva_posicion)){
                juego->homero.posicion = nueva_posicion;
                juego->homero.energia -= 1;

                if(juego->homero.linterna_activada == true){
                    juego->homero.mov_linterna ++;
                }
            }
        break;
        case LINTERNAS:
            if(juego->homero.linterna_activada == false){
                juego->homero.linterna_activada = true;

                if(juego->homero.cantidad_linternas > 0){
                    juego->homero.cantidad_linternas --;
                }
                
                juego->homero.mov_linterna ++;
                mover_ratas(juego);
            }
        break;
    }  
}

/*
 *  Pre condiciones: El juego debe estar inicializado previamente con `inicializar_juego`.
 *  Post condiciones: Si homero choca con una rata radioactiva (está en la misma posición), se le resta 1 a 'cantidad_linternas' de 'homero'.
 */
void chocar_ratas(juego_t * juego){
    int fila_homero = juego->homero.posicion.fil;
    int col_homero = juego->homero.posicion.col;

    for(int i = 0; i < juego->cantidad_obstaculos; i++){
        if(juego->obstaculos[i].tipo == RATAS_RADIOACTIVAS){
            if(juego->obstaculos[i].posicion.fil == fila_homero && juego->obstaculos[i].posicion.col == col_homero && juego->homero.cantidad_linternas > 0){
                juego->homero.cantidad_linternas -= LINTERNA_RATAS;
            }
        }
    }
}   

/*
 *  Pre condiciones: El juego debe estar inicializado previamente con `inicializar_juego`.
 *  Post condiciones: Si homero choca con un barril (está en la misma posición), se le resta 15 puntos a 'energia' de 'homero'.
 */
void chocar_barriles(juego_t * juego){
    int fila_homero = juego->homero.posicion.fil;
    int col_homero = juego->homero.posicion.col;

    for(int i = 0; i < juego->cantidad_obstaculos; i++){
        if(juego->obstaculos[i].tipo == BARRILES){
            if(juego->obstaculos[i].posicion.fil == fila_homero && juego->obstaculos[i].posicion.col == col_homero && juego->homero.energia > 0){
                juego->homero.energia -= PUNTOS_CHOCA_BARRIL;
            }
        }
    }
}

/*
 *  Pre condiciones: -
 *  Post condiciones: Calcula la distancia entre dos posiciones dentro del juego.
 */
int calcula_dist_manhattan(int fil_1, int fil_2, int col_1, int col_2){
   return (abs(fil_1 - fil_2) + abs(col_1 - col_2));
}

/*
 *  Pre condiciones: El juego debe estar inicializado previamente con `inicializar_juego`.
 *  Post condiciones: Muestra los obstáculos, herramientas, barras de plutonio o propulsores que se encuentren a una distancia manhattan 2 
 *  de homero mientras la linterna este encendida.
 */
void mostrar_elem_linterna(juego_t * juego){
    int fila_homero = juego->homero.posicion.fil;
    int col_homero = juego->homero.posicion.col;
        
    for(int i = 0; i < juego->cantidad_herramientas; i++){
        int fil_herramienta = juego->herramientas[i].posicion.fil;
        int col_herramienta = juego->herramientas[i].posicion.col;
            
        int distancia_m = calcula_dist_manhattan(fila_homero, fil_herramienta, col_homero, col_herramienta);
        if(distancia_m <= MAX_DISTANCIA_ELEMENTOS && juego->homero.linterna_activada == true){
            juego->herramientas[i].visible = true;
        } else if(juego->herramientas[i].tipo != INTERRUPTORES) {
            juego->herramientas[i].visible = false;
        }
    }
        
    for(int i = 0; i < juego->cantidad_obstaculos; i++){

        int fil_obstaculo = juego->obstaculos[i].posicion.fil;
        int col_obstaculo = juego->obstaculos[i].posicion.col;
            
        int distancia_m = calcula_dist_manhattan(fila_homero, fil_obstaculo, col_homero, col_obstaculo);
            
        if(distancia_m <= MAX_DISTANCIA_ELEMENTOS && juego->homero.linterna_activada == true){
            juego->obstaculos[i].visible = true;
        } else {
            juego->obstaculos[i].visible = false;
        }
    }
            
    for(int i = 0;  i < juego->cantidad_barras; i++){
        int fil_barra = juego->barras[i].posicion.fil;
        int col_barra = juego->barras[i].posicion.col;
            
        int distancia_m = calcula_dist_manhattan(fila_homero, fil_barra, col_homero, col_barra);
            
        if(distancia_m <= MAX_DISTANCIA_ELEMENTOS && juego->homero.linterna_activada == true){
            juego->barras[i].visible = true;
        } else {
            juego->barras[i].visible = false;
        }
    }
            
    for(int i = 0; i < juego->cantidad_propulsores; i++){
        int fil_propulsor = juego->propulsores[i].posicion.fil;
        int col_propulsor = juego->propulsores[i].posicion.col;
            
        int distancia_m = calcula_dist_manhattan(fila_homero, fil_propulsor, col_homero, col_propulsor);
        if(distancia_m <= MAX_DISTANCIA_ELEMENTOS && juego->homero.linterna_activada == true){
            juego->propulsores[i].visible = true;
        } else {
            juego->propulsores[i].visible = false;
        }
    }
}

/*
 *  Pre condiciones: -
 *  Post condiciones: Mientras la cantidad de movimientos de homero con la linterna encendida sea menor o igual a 5, 
 *  muestra los elementos según la distancia, una vez superada la cantidad de movimientos permitidos, se apaga la linterna, 
 *  se deja de mostrar los elementos y 'mov_linterna' se resetea a 0.
 */
void activar_linterna(juego_t * juego, char accion){
    if(juego->homero.cantidad_linternas >= 0){
        if(juego->homero.linterna_activada == true){
            if(juego->homero.mov_linterna > MOVIMIENTOS_CON_LINTERNA){
                juego->homero.linterna_activada = false;
                juego->homero.mov_linterna = 0;
            }
        }

        mostrar_elem_linterna(juego);
    }
}

/*
 *  Pre condiciones: El juego debe estar inicializado previamente con `inicializar_juego`.
 *  Post condiciones: Devuelve true si homero esta en la misma posición que algún interruptor, en caso contrario, devuelve false.
 */
bool esta_sobre_interruptor(juego_t * juego){
    bool posiciones_iguales = false;

    int fila_homero = juego->homero.posicion.fil;
    int col_homero = juego->homero.posicion.col;

    for(int i = 0; i < juego->cantidad_herramientas; i++){
        int fila_herramienta = juego->herramientas[i].posicion.fil;
        int col_herramienta =  juego->herramientas[i].posicion.col;

        if((juego->herramientas[i].tipo == INTERRUPTORES) && (fila_herramienta == fila_homero && col_herramienta == col_homero)){
            posiciones_iguales = true;
        }
    }

    return posiciones_iguales;
}

/*
 *  Pre condiciones: El juego debe estar inicializado previamente con `inicializar_juego`.
 *  Post condiciones: Si homero se encuentra sobre un interruptor, las herramientas, obstaculos, barras, y propulsores pasan a 
 *  estar visibles.
 */
void activar_interruptor(juego_t * juego){
    bool h_sobre_interruptor = esta_sobre_interruptor(juego);

    if(h_sobre_interruptor){
        for(int i = 0; i < juego->cantidad_herramientas; i++){
            juego->herramientas[i].visible = true;
        }
        
        mover_ratas(juego);
        for(int i = 0; i < juego->cantidad_obstaculos; i++){
            juego->obstaculos[i].visible = true;
        }
        
        for(int i = 0; i < juego->cantidad_barras; i++){
            juego->barras[i].visible = true;
        }
        
        for(int i = 0; i < juego->cantidad_propulsores; i++){
            juego->propulsores[i].visible = true;
        }
    }
}

/*
 *  Pre condiciones: El juego debe estar inicializado previamente con `inicializar_juego`.
 *  Post condiciones: Si homero se encuentra sobre una barra de plutonio (esta en la misma posición), esa barra se elimina del vector 'barras',
 *  y se le suma 1 a 'cantidad_barras' de homero.
 */
void recoger_barra_plutonio(juego_t * juego){
    for(int i = 0; i < juego->cantidad_barras; i++){
        if(juego->homero.posicion.fil == juego->barras[i].posicion.fil && juego->homero.posicion.col == juego->barras[i].posicion.col){
            int posicion_barra = i;
            for(int j = posicion_barra; j < (juego->cantidad_barras - 1); j++){
                juego->barras[j] = juego->barras[j + 1];
            }

            juego->cantidad_barras --;
            juego->homero.cantidad_barras += 1;
        }
    }
}

/*
 *  Pre condiciones: El juego debe estar inicializado previamente con `inicializar_juego`.
 *  Post condiciones: Si homero se encuentra sobre una dona (esta en la misma posición), esa dona se elimina del vector 'herramientas', 
 *  y se le suma 10 a 'energia' de homero.
 */
void recoger_dona(juego_t * juego){
    for(int i = 0; i < juego->cantidad_herramientas; i++){
        if(juego->herramientas[i].tipo == DONAS && (juego->homero.posicion.fil == juego->herramientas[i].posicion.fil && juego->homero.posicion.col == juego->herramientas[i].posicion.col)){
            int posicion_dona = i;
            for(int j = posicion_dona; j < (juego->cantidad_herramientas - 1); j++){
                juego->herramientas[j] = juego->herramientas[j + 1];
            }

            juego->cantidad_herramientas --;
            juego->homero.energia += ENERGIA_DONA;
        }
    }
}

/*
 *  Pre condiciones: El juego debe estar inicializado previamente con `inicializar_juego` y 'accion' debe ser una acción válida.
 *  Post condiciones: Dependiendo la acción ingresada para moverse sobre la escalera, mueve a homero diagonalmente 3 veces 
 *  (o hasta que se llegue al límite del terreno) hacia arriba a la derecha, izquierda, o hacia arriba.
 */
void usar_escalera(juego_t * juego, char accion){
    int fil_homero = juego->homero.posicion.fil;
    int col_homero = juego->homero.posicion.col;

    for(int i = 0; i < juego->cantidad_propulsores; i++){
        int fil_propulsor = juego->propulsores[i].posicion.fil;
        int col_propulsor = juego->propulsores[i].posicion.col;

        if((fil_propulsor == fil_homero && col_propulsor == col_homero) && (juego->propulsores[i].tipo == ESCALERAS)){
            coordenada_t nueva_posicion = juego->homero.posicion;

            switch(accion){
                case DERECHA:
                    for(int i = 0; i < MAX_MOVIMIENTOS_ESCALERA; i++){
                        nueva_posicion.fil --;
                        nueva_posicion.col ++;

                        if(es_movimiento_valido(nueva_posicion)){
                            juego->homero.posicion = nueva_posicion;
                            recoger_barra_plutonio(juego);
                            recoger_dona(juego);
                            chocar_barriles(juego);
                            chocar_ratas(juego);
                        }
                    }
                break;
                case IZQUIERDA:
                    for(int i = 0; i < MAX_MOVIMIENTOS_ESCALERA; i++){
                        nueva_posicion.fil --;
                        nueva_posicion.col --;

                        if(es_movimiento_valido(nueva_posicion)){
                            juego->homero.posicion = nueva_posicion;
                            recoger_barra_plutonio(juego);
                            recoger_dona(juego);
                            chocar_barriles(juego);
                            chocar_ratas(juego);
                        }
                    }
                break;
                case ARRIBA:
                    for(int i = 0; i < MAX_MOVIMIENTOS_ESCALERA; i++){
                        nueva_posicion.fil --;

                        if(es_movimiento_valido(nueva_posicion)){
                            juego->homero.posicion = nueva_posicion;
                            recoger_barra_plutonio(juego);
                            recoger_dona(juego);
                            chocar_barriles(juego);
                            chocar_ratas(juego);
                        }
                    }
                break;
                case ABAJO:
                    for(int i = 0; i < MAX_MOVIMIENTOS_ESCALERA; i++){
                        nueva_posicion.fil --;

                        if(es_movimiento_valido(nueva_posicion)){
                            juego->homero.posicion = nueva_posicion;
                            recoger_barra_plutonio(juego);
                            recoger_dona(juego);
                            chocar_barriles(juego);
                            chocar_ratas(juego);
                        }
                    }
                break;
            }
        }
    }
}

/*
 *  Pre condiciones: El juego debe estar inicializado previamente con `inicializar_juego` y 'accion' debe ser una acción válida.
 *  Post condiciones: Dependiendo la acción ingresada para moverse sobre un charco, mueve a homero diagonalmente 3 veces 
 *  (o hasta que se llegue al límite del terreno) hacia abajo a la derecha, izquierda, o hacia abajo.
 */
void usar_charco(juego_t * juego, char accion){
    int fil_homero = juego->homero.posicion.fil;
    int col_homero = juego->homero.posicion.col;

    for(int i = 0; i < juego->cantidad_propulsores; i++){
        int fil_propulsor = juego->propulsores[i].posicion.fil;
        int col_propulsor = juego->propulsores[i].posicion.col;

        if((fil_propulsor == fil_homero && col_propulsor == col_homero) && (juego->propulsores[i].tipo == CHARCOS)){
            coordenada_t nueva_posicion = juego->homero.posicion;

            switch(accion){
                case DERECHA: 
                    for(int i = 0; i < MAX_MOVIMIENTOS_CHARCO; i++){
                        nueva_posicion.fil ++;
                        nueva_posicion.col ++;

                        if(es_movimiento_valido(nueva_posicion)){
                            juego->homero.posicion = nueva_posicion;
                            recoger_barra_plutonio(juego);
                            recoger_dona(juego);
                            chocar_barriles(juego);
                            chocar_ratas(juego);
                        }
                    }
                break;
                case IZQUIERDA:
                    for(int i = 0; i < MAX_MOVIMIENTOS_CHARCO; i++){
                        nueva_posicion.fil ++;
                        nueva_posicion.col --;

                        if(es_movimiento_valido(nueva_posicion)){
                            juego->homero.posicion = nueva_posicion;
                            recoger_barra_plutonio(juego);
                            recoger_dona(juego);
                            chocar_barriles(juego);
                            chocar_ratas(juego);
                        }
                    }
                break;
                case ARRIBA:
                for(int i = 0; i < MAX_MOVIMIENTOS_CHARCO; i++){
                    nueva_posicion.fil ++;

                    if(es_movimiento_valido(nueva_posicion)){
                        juego->homero.posicion = nueva_posicion;
                        recoger_barra_plutonio(juego);
                        recoger_dona(juego);
                        chocar_barriles(juego);
                        chocar_ratas(juego);
                    }
                }
                break;
                case ABAJO:
                    for(int i = 0; i < MAX_MOVIMIENTOS_CHARCO; i++){
                        nueva_posicion.fil ++;

                        if(es_movimiento_valido(nueva_posicion)){
                            juego->homero.posicion = nueva_posicion;
                            recoger_barra_plutonio(juego);
                            recoger_dona(juego);
                            chocar_barriles(juego);
                            chocar_ratas(juego);
                        }
                    }
                break;
            }
        }
    }
}

/*
 *  Pre condiciones: El juego debe estar inicializado previamente con `inicializar_juego` y la acción
 *  debe ser válida.
 *  Post condiciones: Realizará la acción recibida por parámetro actualizando el juego.
 */
void realizar_jugada(juego_t * juego, char accion){
    realizar_movimiento(juego, accion);
    chocar_ratas(juego);
    chocar_barriles(juego);
    recoger_barra_plutonio(juego);
    recoger_dona(juego);
    usar_escalera(juego, accion);
    usar_charco(juego, accion);
    activar_linterna(juego, accion);
    activar_interruptor(juego);
}

/*
 *  Pre condiciones: -
 *  Post condiciones: Muestra por pantalla informacón útil para el usuario al momento de jugar.
 */
void mostrar_info_util(juego_t juego){
    printf(AZUL ". ENERGÍA DISPONIBLE:" NORMAL " %i\n",juego.homero.energia);
    printf(AZUL ". CANTIDAD DE LINTERNAS:" NORMAL " %i\n", juego.homero.cantidad_linternas);
    printf(AZUL ". BARRAS DE PLUTONIO RECOGIDAS:" NORMAL " %i\n", juego.homero.cantidad_barras);
    printf("\n");

    printf(AZUL "> " NORMAL SUBRAYADO "Elementos" NORMAL ":");
    printf(AZUL "\t\t\t      > " NORMAL SUBRAYADO "Movimientos" NORMAL ": \n\n");
    printf("   INTERRUPTORES " ROJO_FUERTE "[%c]" NORMAL, INTERRUPTORES);
    printf("\t\t\t %s  ARRIBA " AZUL_FUERTE "[%c]" NORMAL"\n", ARRIBA_EMOJI, ARRIBA);
    printf("   BARRAS DE PLUTONIO " AZUL_FUERTE "[%c]" NORMAL, BARRAS);
    printf("\t\t %s  ABAJO " AZUL_FUERTE "[%c]" NORMAL "\n", ABAJO_EMOJI, ABAJO);
    printf("   DONAS " ROSA_FUERTE "[%c]" NORMAL, DONAS);
    printf("\t\t\t\t %s  IZQUIERDA " AZUL_FUERTE "[%c]" NORMAL "\n", IZQUIERDA_EMOJI, IZQUIERDA);
    printf("   BARRILES "VERDE_FUERTE "[%c]" NORMAL, BARRILES);
    printf("\t\t\t\t %s  DERECHA " AZUL_FUERTE "[%c]" NORMAL "\n", DERECHA_EMOJI, DERECHA);
    printf("   RATAS RADIOACTIVAS " BLANCO_FUERTE "[%c]" NORMAL, RATAS_RADIOACTIVAS);
    printf("\t\t %s LINTERNA " AZUL_FUERTE "[%c]" NORMAL "\n", LINTERNA_EMOJI, LINTERNAS);
    printf("   CHARCOS " CELESTE_FUERTE "[%c]" NORMAL "\n", CHARCOS);
    printf("   ESCALERAS " CELESTE_FUERTE "[%c]" NORMAL "\n\n", ESCALERAS);
}

/*
 *  Pre condiciones: La matriz 'terreno' debe estar inicializada correctamente y el juego debe estar inicializado previamente con `inicializar_juego`.
 *  Post condiciones: Carga la matriz con todos los elementos del juego.
 */
void llenar_terreno(juego_t juego, char terreno[MAX_FILAS][MAX_COLUMNAS]){
    for(int i = 0; i < juego.cantidad_herramientas; i++){
        if(juego.herramientas[i].visible == true){
            int fil = juego.herramientas[i].posicion.fil;
            int col = juego.herramientas[i].posicion.col;

            terreno[fil][col] = juego.herramientas[i].tipo;
        }
    }

    for(int i = 0; i < juego.cantidad_obstaculos; i++){
        if(juego.obstaculos[i].visible == true){
            int fil = juego.obstaculos[i].posicion.fil;
            int col = juego.obstaculos[i].posicion.col;

            terreno[fil][col] = juego.obstaculos[i].tipo;
        }
    }

    for(int i = 0; i < juego.cantidad_barras; i++){
        if(juego.barras[i].visible == true){
            int fil = juego.barras[i].posicion.fil;
            int col = juego.barras[i].posicion.col;

            terreno[fil][col] = juego.barras[i].tipo;
        }
    }

    for(int i = 0; i < juego.cantidad_propulsores; i++){
        if(juego.propulsores[i].visible == true){
            int fil = juego.propulsores[i].posicion.fil;
            int col = juego.propulsores[i].posicion.col;

            terreno[fil][col] = juego.propulsores[i].tipo;
        }
    }
    
    int fil_homero = juego.homero.posicion.fil;
    int col_homero = juego.homero.posicion.col;
    terreno[fil_homero][col_homero] = HOMERO;
}

/*
 *  Pre condiciones: La matriz 'terreno' debe estar previamente cargada por 'llenar_terreno' con todos los elementos del juego.
 *  Post condiciones: Imprime la matriz 'terreno' con todos sus elementos, cada uno con colores diferentes.
 */
void mostrar_elem_con_color(char terreno[MAX_FILAS][MAX_COLUMNAS]){

    printf("\n"); 
    for(int i = 0; i < MAX_FILAS; i++){
        for(int j = 0; j < MAX_COLUMNAS; j++){
            switch(terreno[i][j]){
                case HOMERO: 
                    printf("|" AMARILLO_FUERTE " %c  " NORMAL , terreno[i][j]);
                break;
                case INTERRUPTORES: 
                    printf("|" ROJO_FUERTE " %c  " NORMAL, terreno[i][j]);
                break;
                case BARRAS:
                    printf("|" AZUL_FUERTE " %c  " NORMAL, terreno[i][j]);
                break;
                case DONAS:
                    printf("|" ROSA_FUERTE" %c  " NORMAL, terreno[i][j]);
                break;
                case ESCALERAS:
                    printf("|" CELESTE_FUERTE " %c  " NORMAL, terreno[i][j]);
                break;
                case RATAS_RADIOACTIVAS:
                    printf("|" BLANCO_FUERTE " %c  " NORMAL, terreno[i][j]);
                break;
                case CHARCOS:
                    printf("|" CELESTE_FUERTE " %c  " NORMAL, terreno[i][j]);
                break;
                case BARRILES:
                    printf("|" VERDE_FUERTE " %c  " NORMAL, terreno[i][j]);
                break;
                default:
                    printf("| %c  ", terreno[i][j]);
                break;
            }
        }
        printf("|\n");
    }
}

/*
 *  Pre condiciones: El juego debe estar inicializado previamente con `inicializar_juego`.
 *  Post condiciones: Imprime el juego por pantalla.
 */
void mostrar_juego(juego_t juego){
    char terreno[MAX_FILAS][MAX_COLUMNAS];
    for(int i = 0; i < MAX_FILAS; i++){
        for(int j = 0; j < MAX_COLUMNAS; j++){
            terreno[i][j] = '.';
        }
    }

    llenar_terreno(juego, terreno);
    mostrar_elem_con_color(terreno);
    printf("\n"); 
    mostrar_info_util(juego);
    printf("\n"); 
}



/*
 *  Pre condiciones: El juego deberá estar inicializado previamente con `inicializar_juego`
 *  Post condiciones: Devuelve:
 *  --> 1 si es ganado
 *  --> -1 si es perdido
 *  --> 0 si se sigue jugando
 *  El juego se dará por ganado cuando Homero junta todas las barras de plutonio.
 *  Se dará por perdido si se le termina la energía a Homero y no juntó todas las barras de plutonio.
 */
int estado_juego(juego_t juego){
    int estado = 3;

    if(juego.homero.cantidad_barras == CANT_BARRAS){
        estado = GANADO;
    } else if(juego.homero.energia <= SIN_ENERGIA && juego.homero.cantidad_barras != CANT_BARRAS){
        estado = PERDIDO;
    } else if(juego.homero.cantidad_barras <= CANT_BARRAS && juego.homero.energia != SIN_ENERGIA && juego.homero.energia > SIN_ENERGIA ){
        estado = SIGUE_JUGANDO;
    }

    return estado;
}