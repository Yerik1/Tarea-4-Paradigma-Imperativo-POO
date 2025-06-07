#ifndef ENEMIGOS_H
#define ENEMIGOS_H

#include "jugador.h"


#define MAX_ENEMIGOS 10

typedef struct GameState GameState;

typedef enum { YETI, FOCA, AVE, HIELO } TipoEnemigo;
typedef enum { DERECHA, IZQUIERDA, ARRIBA, ABAJO } Direccion;

typedef struct {
    TipoEnemigo tipo;
    int fila;
    int columna;
    Direccion direccion;
    int activo;
} Enemigo;

void inicializar_enemigos(struct GameState* game);
void crear_ave(struct GameState* game, int fila);
void crear_hielo(struct GameState* game, int fila, int columna);
void crear_yeti(struct GameState* game, int fila, Direccion direccion);
void crear_foca(struct GameState* game, int fila, Direccion direccion);
void verificar_colision_jugadores_enemigos(struct GameState* game, int* proximos_movimientos);
void mover_enemigos(struct GameState* game, int* proximos_movimientos);
void imprimir_enemigos(struct GameState* game);

#endif
