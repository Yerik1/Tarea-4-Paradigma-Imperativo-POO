#ifndef JUGADOR_H
#define JUGADOR_H

typedef struct GameState GameState;

typedef struct {
    int fila;
    int columna;
    // ...otros campos si los tienes...
} Jugador;



void inicializar_jugador(struct GameState* game, int idx, int fila, int columna);
void mover_jugador(struct GameState* game, int idx, int movimiento);

#endif
