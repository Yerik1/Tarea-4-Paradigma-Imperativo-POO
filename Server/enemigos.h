#ifndef ENEMIGOS_H
#define ENEMIGOS_H

typedef struct Partida Partida;

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

void inicializar_enemigos(Partida* game);
void crear_ave(Partida* game, int fila);
void crear_hielo(Partida* game, int fila, int columna);
void crear_yeti(Partida* game, int fila, Direccion direccion);
void crear_foca(Partida* game, int fila, Direccion direccion);
void verificar_colision_jugadores_enemigos(Partida* game, int* proximos_movimientos);
void mover_enemigos(Partida* game, int* proximos_movimientos);
void imprimir_enemigos(Partida* game);

#endif
