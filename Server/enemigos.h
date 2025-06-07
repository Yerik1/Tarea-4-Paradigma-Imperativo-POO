#ifndef ENEMIGOS_H
#define ENEMIGOS_H

#include "jugador.h"

#define MAX_ENEMIGOS 10

typedef enum { YETI, FOCA, AVE, HIELO } TipoEnemigo;
typedef enum { DERECHA, IZQUIERDA, ARRIBA, ABAJO } Direccion;

typedef struct {
    TipoEnemigo tipo;
    int fila;
    int columna;
    Direccion direccion;
    int activo;
} Enemigo;

extern Enemigo enemigos[MAX_ENEMIGOS];
extern const int MAX_ENEMIGOS_CONST;

void inicializar_enemigos();
void crear_ave(int fila);
void crear_hielo(int fila, int columna);
void crear_yeti(int fila, Direccion direccion);
void crear_foca(int fila, Direccion direccion);
void verificar_colision_jugadores_enemigos(Jugador* jugadores, int num_jugadores, int* proximos_movimientos);
void mover_enemigos(Jugador* jugadores, int num_jugadores, int* proximos_movimientos);

#endif
